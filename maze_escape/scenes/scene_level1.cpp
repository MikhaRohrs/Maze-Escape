#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_timer.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
shared_ptr<TimerComponent> timerText;
static shared_ptr<Entity> timer;

sf::Texture playerTexture;

void Level1Scene::Load() {
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/levels/testLevel.txt", 40.0f);

  if (!playerTexture.loadFromFile("res/img/invaders_sheet.png"))
  {
      cout << "Could not load texture\n";
  }

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, ho));

  // Create player
  {
      auto playerSize = Vector2f(20.0f, 30.0f);
      player = makeEntity();
      player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
      auto s = player->addComponent<ShapeComponent>();
      s->setShape<sf::RectangleShape>(playerSize);
      s->getShape().setFillColor(Color::Transparent);
      s->getShape().setOrigin(playerSize / 2.0f);

      player->addComponent<PlayerPhysicsComponent>(playerSize);

      auto playerSprite = player->addComponent<SpriteComponent>();
      playerSprite->setTexture(make_shared<Texture>(playerTexture));
      playerSprite->setTextureRect(IntRect(Vector2(0, 0), Vector2(20, 30)));
      playerSprite->setOrigin(s->getShape().getOrigin());
  }
    // Create timer text
    {
    timer = makeEntity();
    timerText = timer->addComponent<TimerComponent>();
    timerText->GetTextObject().setPosition({ 5, 0 });

    timerText->GetTextObject().setFillColor(Color::Black);
    timerText->GetTextObject().setOutlineColor(Color::White);

    timerText->GetTextObject().setOutlineThickness(2);
    timerText->GetTextObject().setCharacterSize(25);
    }

  // Add physics colliders to level tiles.
  {
    auto walls = ls::findTiles(ls::WALL);
    for (auto w : walls) {
      auto pos = ls::getTilePosition(w);
      pos += Vector2f(20.f, 20.f); //offset to center
      auto e = makeEntity();
      e->setPosition(pos);
      e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
    }
  }

  //Simulate long loading times
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  cout << " Scene 1 Load Done" << endl;

  setLoaded(true);
}

void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  player.reset();
  timer.reset();
  timerText.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt)
{
  if (ls::getTileAt(player->getPosition()) == ls::END) {
    Engine::ChangeScene((Scene*)&level2);
  }
  else if (timerText->GetCurrentTime() <= 0) { Engine::ChangeScene((Scene*)&loseGame); } // <-- todo: investigate nullptr error from scene change
  Scene::Update(dt);
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow(), player->getPosition());
  Scene::Render();
}
