#include "engine.h"
#include "maths.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
#include <stdexcept>

using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;

static bool loading = false; // Loading new scene
static float loadTextOpacity = 0.f; // Rapidly increments/decrement between 50 and 250, sets opacity of loading text.
static float loadingTime = 0.f; // Time taken so far to load the next scene.
static bool gainingOpacity = true; // Bool that tracks if the opacity of the loading text should be incrementing or decrementing.

static RectangleShape loadingBar(Vector2f(100, 10)); // Create the bar (Doing this outside the render function to prevent initializing this object per frame.
static Text t("L O A D I N G . . .", *Resources::get<sf::Font>("RobotoMono-Regular.ttf")); // Loading text object with pre-defined font.

static RenderWindow* _window; // Game window.

/**
 * \brief Loading menu update function where the loading text's opacity will cycle and check if the currently-loading scene has fully loaded.
 * \param dt Delta time.
 * \param scn Scene being loaded.
 */
void Loading_update(float dt, const Scene* const scn) {

    // Stop loading if scene is fully loaded.
  if (scn->isLoaded()) 
  {
    cout << "Eng: Exiting Loading Screen\n";
    loading = false;
  }
    // If scene is not loaded, calculate new opacity of the loading text, increment the loading time to be used in loading screen render.
  else 
  {
  	loadTextOpacity += (gainingOpacity) ? 300 * dt : -300 * dt;
  	if (loadTextOpacity < 50) { gainingOpacity = true; }
  	else if (loadTextOpacity > 200) { gainingOpacity = false; }
  	loadingTime += dt;
  }
}

/**
 * \brief Render a red bar at the bottom of the screen, visually moving to represent loading progress.
 * \n Also, create a text that says "Loading..." which slowly flashes to show that the program is running and loading the next level.
 */
void Loading_render() {
    // Set loading bar position, scale it equal to 10 * loading time (Bar moves to the right, acting as a loading bar).
	loadingBar.setPosition(Vector2f(0, Engine::getWindowSize().y));
    loadingBar.setOrigin(Vector2f(50, 10));
    loadingBar.setScale(Vector2f(10 * loadingTime, 1));
	loadingBar.setFillColor(Color::Red);

    // Set text fill colour, character size and position.
	t.setFillColor(Color(255,255,255, loadTextOpacity));
	t.setCharacterSize(40);
	t.setPosition(Vector2f(Engine::getWindowSize().x * 0.3, Engine::getWindowSize().y * 0.7));

    // Queue the loading text and bar to the system renderer.
	Renderer::queue(&t);
	Renderer::queue(&loadingBar);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update() {
  static sf::Clock clock;
  float dt = clock.restart().asSeconds();
  {
    frametimes[++ftc] = dt;
    static string avg = _gameName + " FPS:";
    if (ftc % 60 == 0) {
      double davg = 0;
      for (const auto t : frametimes) {
        davg += t;
      }
      davg = 1.0 / (davg / 255.0);
      _window->setTitle(avg + toStrDecPt(2, davg));
    }
  }

  if (loading) {
    Loading_update(dt, _activeScene);
  } else if (_activeScene != nullptr) {
    Physics::update(dt);
    _activeScene->Update(dt);
  }
}

void Engine::Render(RenderWindow& window) {
  if (loading) {
    Loading_render();
  } else if (_activeScene != nullptr) {
    _activeScene->Render();
  }

  Renderer::render();
}

void Engine::Start(unsigned int width, unsigned int height,
                   const std::string& gameName, Scene* scn) {
  RenderWindow window(VideoMode(width, height), gameName);
  _gameName = gameName;
  _window = &window;
  Renderer::initialise(window);
  Physics::initialise();
  ChangeScene(scn);
  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed) {
        window.close();
      }
    }
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
      window.close();
    }

    window.clear();
    Update();
    Render(window);
    window.display();
  }
  if (_activeScene != nullptr) {
    _activeScene->UnLoad();
    _activeScene = nullptr;
  }
  window.close();
  Physics::shutdown();
  // Render::shutdown();
}

std::shared_ptr<Entity> Scene::makeEntity() {
  auto e = make_shared<Entity>(this);
  ents.list.push_back(e);
  return std::move(e);
}

void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }

void Engine::ChangeScene(Scene* s) {
  cout << "Eng: changing scene: " << s << endl;
  auto old = _activeScene;
  _activeScene = s;

  if (old != nullptr) {
    old->UnLoad(); // todo: Unload Async
  }

  if (!s->isLoaded()) {
    cout << "Eng: Entering Loading Screen\n";
    loadingTime =0;
    _activeScene->LoadAsync();
    //_activeScene->LoadSync(); <-- USE THIS WHEN DEBUGGING SCENE ERRORS!
    loading = true;
  }
}

void Scene::Update(const double& dt) { ents.update(dt); }

void Scene::Render() { ents.render(); }

bool Scene::isLoaded() const {
  {
    std::lock_guard<std::mutex> lck(_loaded_mtx);
    // Are we already loading asynchronously?
    if (_loaded_future.valid() // yes
        &&                     // Has it finished?
        _loaded_future.wait_for(chrono::seconds(0)) ==
            future_status::ready) {
      // Yes
      _loaded_future.get();
      _loaded = true;
    }
    return _loaded;
  }
}
void Scene::setLoaded(bool b) {
  {
    std::lock_guard<std::mutex> lck(_loaded_mtx);
    _loaded = b;
  }
}

void Scene::UnLoad() {
  ents.list.clear();
  setLoaded(false);
}

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

namespace timing {
// Return time since Epoc
long long now() {
  return std::chrono::high_resolution_clock::now()
      .time_since_epoch()
      .count();
}
// Return time since last() was last called.
long long last() {
  auto n = now();
  static auto then = now();
  auto dt = n - then;
  then = n;
  return dt;
}
} // namespace timing

Scene::~Scene() { UnLoad(); }