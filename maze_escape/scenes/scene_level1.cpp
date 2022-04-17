#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
float timer = 15.0f;
shared_ptr<TextComponent> text;

void Level1Scene::Load() {
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/levels/testLevel.txt", 40.0f);

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, ho));

  // Create player
  {
      player = makeEntity();
      player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
      auto s = player->addComponent<ShapeComponent>();
      s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
      s->getShape().setFillColor(Color::Magenta);
      s->getShape().setOrigin(Vector2f(10.f, 15.f));

      player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
  }
    // Create timer text
    {
    auto timerText = makeEntity();
    text = timerText->addComponent<TextComponent>();

    text->GetTextObject().setPosition({ 5, 0 });

    text->GetTextObject().setFillColor(Color::Black);
    text->GetTextObject().setOutlineColor(Color::White);

    text->GetTextObject().setOutlineThickness(1);
    text->GetTextObject().setCharacterSize(24);
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
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt)
{
    // Decrement timer by delta time.
    timer -= dt;

    // If time is under 11 seconds (In this case, it is treated as true starting from 10 seconds remaining on the visible, in-game clock), make the timer flash
    // red to indicate that the player is running out of time.
    if(timer <= 11)
    {
        bool evenNumber = (static_cast<int>(timer) % 2 == 0) ? true : false;
        if (evenNumber) { text->GetTextObject().setFillColor(Color::Red); }
        else { text->GetTextObject().setFillColor(Color::Black); }
    }

    // Cosmetic change where the text is shifted very slightly to the right when it reaches single digit numbers (As the number text is left-aligned).
    if(timer <= 10){ text->GetTextObject().setPosition(Vector2f(14, 0)); }

    // Each frame update the timer text to show the remaining time, casted to an integer without rounding.
    text->SetText(std::to_string(static_cast<int>(timer)));

  if (ls::getTileAt(player->getPosition()) == ls::END) {
    Engine::ChangeScene((Scene*)&level2);
  }
  Scene::Update(dt);
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
