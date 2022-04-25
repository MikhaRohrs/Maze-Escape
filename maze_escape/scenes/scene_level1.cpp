#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_timer.h"
#include "../components/cmp_weapon.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<ShapeComponent> playerShape;

shared_ptr<TimerComponent> timerText;
static shared_ptr<Entity> timer;

static shared_ptr<Entity> weapon;
static shared_ptr <ShapeComponent> weaponShape;

bool pickedUpWeapon = false;

void Level1Scene::Load() {
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/levels/testLevel.txt", 40.0f);

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, ho));

  // Create player
  {
      player = makeEntity();
      player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
      playerShape = player->addComponent<ShapeComponent>();
      playerShape->setShape<sf::RectangleShape>(Vector2f(20.f, 20.f));
      playerShape->getShape().setFillColor(Color::Magenta);
      playerShape->getShape().setOrigin(Vector2f(10.f, 10.f));

      player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 20.f));
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

    // Create weapon powerup
  {
      weapon = makeEntity();
      weapon->setPosition(ls::getTilePosition(ls::findTiles(ls::WEAPON)[0]));
      weaponShape = weapon->addComponent<ShapeComponent>();
      weaponShape->setShape<sf::RectangleShape>(Vector2f(15.f, 40.f));
      weaponShape->getShape().setPosition(weapon->getPosition());
      weaponShape->getShape().setFillColor(Color::Cyan);
      weaponShape->getShape().setOrigin(Vector2f(5.f, 10.f));
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
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt)
{
    if (ls::getTileAt(player->getPosition()) == ls::END) 
    {
        Engine::ChangeScene((Scene*)&level2);
    }

	if(!pickedUpWeapon && playerShape->getShape().getGlobalBounds().contains(weaponShape->getShape().getGlobalBounds().getPosition()))
	{
  		auto newWeapon = player->addComponent<PlayerWeaponComponent>();
		printf("hit\n");
		weapon->setVisible(false);
		pickedUpWeapon = true;
	}

  if (timerText->GetCurrentTime() <= 0) { Engine::ChangeScene(&loseGame); }
  Scene::Update(dt);
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow(), player->getPosition());
  Scene::Render();
}