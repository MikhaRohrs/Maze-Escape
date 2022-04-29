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


// Player entity and attached shape component
static shared_ptr<Entity> player;
static shared_ptr<ShapeComponent> playerShape;

// Timer entity and attached timer component
shared_ptr<TimerComponent> timerText;
static shared_ptr<Entity> timer;

// Weapon shape component, used to determine if the player picked it up
shared_ptr<Entity> weapon;
static shared_ptr <ShapeComponent> weaponShape;

bool pickedUpWeapon;

void Level1Scene::Load() {
	cout << " Scene 1 Load" << endl;
	ls::loadLevelFile("res/levels/testLevel.txt", 40.0f);

	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
	ls::setOffset(Vector2f(0, ho));

	// Create player
	player = makeEntity();
	player->addTag("player");
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	playerShape = player->addComponent<ShapeComponent>();
    playerShape->setShape<sf::RectangleShape>(Vector2f(20.f, 20.f));
    playerShape->getShape().setFillColor(Color::Magenta);
    playerShape->getShape().setOrigin(Vector2f(10.f, 10.f));

    player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 20.f));

    // Create timer text
    timer = makeEntity();
	timer->addTag("timer");
    timerText = timer->addComponent<TimerComponent>();
    timerText->GetTextObject().setPosition({ 5, 0 });

    timerText->GetTextObject().setFillColor(Color::Black);
    timerText->GetTextObject().setOutlineColor(Color::White);

    timerText->GetTextObject().setOutlineThickness(2);
    timerText->GetTextObject().setCharacterSize(25);

	// Add physics colliders to level tiles.
	{
		auto walls = ls::findTiles(ls::WALL);
		for (auto w : walls) 
        {
			auto pos = ls::getTilePosition(w);
			pos += Vector2f(20.f, 20.f); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
		}
	}

    // Create weapon powerup
	weapon = makeEntity();
	weapon->setPosition(ls::getTilePosition(ls::findTiles(ls::WEAPON)[0]));
	weaponShape = weapon->addComponent<ShapeComponent>();
	weaponShape->setShape<sf::RectangleShape>(Vector2f(20.f, 40.f));
	weaponShape->getShape().setPosition(weapon->getPosition());
	weaponShape->getShape().setFillColor(Color::Cyan);
	weaponShape->getShape().setOrigin(Vector2f(10.f, 20.f));

    pickedUpWeapon = false;

	// Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << " Scene 1 Load Done" << endl;

	setLoaded(true);
}

// Clear out entities, unload components, and unload the tile map.
void Level1Scene::UnLoad() 
{
	cout << "Scene 1 Unload" << endl;
	player.reset();
	timer.reset();
	playerShape.reset();
	timerText.reset();
	weaponShape.reset();
	weapon.reset();
	ls::unload();
	Scene::UnLoad();
}

void Level1Scene::Update(const double& dt)
{
    if (ls::getTileAt(player->getPosition()) == ls::END) 
    {
        Engine::ChangeScene((Scene*)&level2);
    }

    // If the player touches the weapon powerup, give the player a weapon and remove the powerup shape from the game, to prevent multiple weapons being picked up.
	// Also adds 10 seconds to the timer.
    if (!pickedUpWeapon && playerShape->getShape().getGlobalBounds().findIntersection(weaponShape->getShape().getGlobalBounds()))
	{
		timerText->ChangeTime(10.f);
		weapon->setForDelete();
    	auto newWeapon = player->addComponent<PlayerWeaponComponent>();
		pickedUpWeapon = true;
	}

    // If the player runs out of time, end the game, player loses.
  if (timerText->GetCurrentTime() <= 0) { Engine::ChangeScene(&loseGame); }
  Scene::Update(dt);
}

void Level1Scene::Render() {

    // For each entity, check if it is at least 150 units or less away from the player. Render only entities that meet this criteria.
    for each(std::shared_ptr<Entity> e in ents.list)
    {
        // Leave timer entity alone, as it should always be rendered.
        if (e == timer) { continue; }
        Vector2f playerDistance = e->getPosition() - player->getPosition();
		playerDistance.x = abs(playerDistance.x); playerDistance.y = abs(playerDistance.y);
        playerDistance.length() > 150 ? e->setVisible(false) : e->setVisible(true);
    }
    
	ls::render(Engine::GetWindow(), player->getPosition());
	Scene::Render();
}