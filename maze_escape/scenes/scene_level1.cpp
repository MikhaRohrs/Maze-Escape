#include "scene_level1.h"

#include <fstream>

#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_timer.h"
#include "../components/cmp_weapon.h"
#include "../components/cmp_powerup_handler.h"
#include "../components/cmp_steering.h"
#include "../components/cmp_pathfinding.h"
#include "../game.h"
#include <LevelSystem.h>
#include "../enemy_ai/aStarPathfinding.h"
#include <iostream>
#include <thread>
#include "SFML/Window/Keyboard.hpp"

using namespace std;
using namespace sf;


shared_ptr<Entity> anEnemy;
shared_ptr<PathfindingComponent> ai;


// Player entity
static shared_ptr<Entity> player;
static shared_ptr<ShapeComponent> playerShape;

// Timer entity
shared_ptr<TimerComponent> timerText;
static shared_ptr<Entity> timer;

// Weapon entity
shared_ptr<Entity> weapon;
static shared_ptr <ShapeComponent> weaponShape;
bool pickedUpWeapon;

Texture playerTexture;
shared_ptr<SpriteComponent> playerSprite;

// Speed powerup entities
vector<shared_ptr<Entity>> speedPowerups;
vector<shared_ptr<ShapeComponent>> speedPowerupShapes;

// Map powerup entities
vector<shared_ptr<Entity>> mapPowerups;
vector<shared_ptr<ShapeComponent>> mapPowerupShapes;

// Ammo Refill powerup entities
vector<shared_ptr<Entity>> ammoPowerups;
vector<shared_ptr<ShapeComponent>> ammoPowerupShapes;

// Sight range of the player. Handled by powerup manager component, but better to keep a local copy of the current sight range
// instead of accessing the component per frame on the render function.
float renderRange;

void Level1Scene::Load() {
	cout << " Scene 1 Load" << endl;
	ls::loadLevelFile("res/levels/testLevel.txt", 40.0f);

	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
	ls::setOffset(Vector2f(0, ho));

	if (!playerTexture.loadFromFile("res/img/maze_player_sheet.png"))
	{
	      cout << "Could not load texture\n";
	}

  // Create player
  {
      player = makeEntity();
      player->addTag("player");
      player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
      playerShape = player->addComponent<ShapeComponent>();
      playerShape->setShape<sf::RectangleShape>(Vector2f(20.f, 20.f));
      playerShape->getShape().setFillColor(Color::Transparent);
      playerShape->getShape().setOrigin(Vector2f(10.f, 10.f));

      player->addComponent<PlayerPhysicsComponent>(Vector2f(20.0f, 20.0f));
	  player->addComponent<PowerupManagerComponent>();
	  player->addComponent<PlayerWeaponComponent>();

      playerSprite = player->addComponent<SpriteComponent>();
      playerSprite->setTexture(make_shared<Texture>(playerTexture));
      playerSprite->setTextureRect(IntRect(Vector2(0, 0), Vector2(20, 30)));
      playerSprite->setOrigin(playerShape->getShape().getOrigin());
      playerSprite->getSprite().scale(Vector2f(2, 2));
  }

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
	weaponShape->getShape().setFillColor(Color::Transparent);
	weaponShape->getShape().setOrigin(Vector2f(10.f, 20.f));

    auto weaponSprite = weapon->addComponent<SpriteComponent>();
    weaponSprite->setTexture(make_shared<Texture>(playerTexture));
    weaponSprite->setTextureRect(IntRect(Vector2(76, 0), Vector2(10, 10)));
    weaponSprite->setOrigin(Vector2f(5.0f, 5.0f));
    weaponSprite->getSprite().scale(Vector2f(3, 3));


    pickedUpWeapon = false;

	// Create speed boost powerups
	for(int i = 0; i < ls::findTiles(ls::POWERUP_SPEED).size(); i++)
	{
		auto powerupPos = ls::getTilePosition(ls::findTiles(ls::POWERUP_SPEED)[i]);

		auto speedPowerup = makeEntity();
		speedPowerup->setPosition(powerupPos);

		auto speedPowerupShape = speedPowerup->addComponent<ShapeComponent>();
		speedPowerupShape->setShape<sf::RectangleShape>(Vector2f(10.f, 20.f));
		speedPowerupShape->getShape().setPosition(speedPowerup->getPosition());
		speedPowerupShape->getShape().setFillColor(Color::Green);
		speedPowerupShape->getShape().setOrigin(Vector2f(5.f, 10.f));

		speedPowerups.push_back(speedPowerup);
		speedPowerupShapes.push_back(speedPowerupShape);
	}

	// Create map powerups
	for (int i = 0; i < ls::findTiles(ls::POWERUP_MAP).size(); i++)
	{
		auto powerupPos = ls::getTilePosition(ls::findTiles(ls::POWERUP_MAP)[i]);

		auto mapPowerup = makeEntity();
		mapPowerup->setPosition(powerupPos);

		auto mapPowerupShape = mapPowerup->addComponent<ShapeComponent>();
		mapPowerupShape->setShape<sf::RectangleShape>(Vector2f(10.f, 20.f));
		mapPowerupShape->getShape().setPosition(mapPowerup->getPosition());
		mapPowerupShape->getShape().setFillColor(Color::Yellow);
		mapPowerupShape->getShape().setOrigin(Vector2f(5.f, 10.f));

		mapPowerups.push_back(mapPowerup);
		mapPowerupShapes.push_back(mapPowerupShape);
	}

	// Create map powerups
	for (int i = 0; i < ls::findTiles(ls::POWERUP_AMMO).size(); i++)
	{
		auto powerupPos = ls::getTilePosition(ls::findTiles(ls::POWERUP_AMMO)[i]);

		auto ammoPowerup = makeEntity();
		ammoPowerup->setPosition(powerupPos);

		auto ammoPowerupShape = ammoPowerup->addComponent<ShapeComponent>();
		ammoPowerupShape->setShape<sf::RectangleShape>(Vector2f(10.f, 20.f));
		ammoPowerupShape->getShape().setPosition(ammoPowerup->getPosition());
		ammoPowerupShape->getShape().setFillColor(Color::White);
		ammoPowerupShape->getShape().setOrigin(Vector2f(5.f, 10.f));

		ammoPowerups.push_back(ammoPowerup);
		ammoPowerupShapes.push_back(ammoPowerupShape);
	}

	anEnemy = makeEntity();
	anEnemy->setPosition(Vector2f(player->getPosition() + Vector2f(50, 50)));
	auto s = anEnemy->addComponent<ShapeComponent>();
	s->setShape<RectangleShape>(Vector2f(10.f, 10.f));
	s->getShape().setFillColor(Color::Cyan);

	auto path = pathFind(Vector2i(0, 1),
		Vector2i(player->getPosition()));
	ai = anEnemy->addComponent<PathfindingComponent>();
	auto w = anEnemy->addComponent<SteeringComponent>();

	// Set initial render / sight range to 150 (from the player).
	renderRange = 150.f;

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
	for( int i = 0; i < speedPowerups.size(); i++)
	{
		speedPowerups[i].reset();
		speedPowerupShapes[i].reset();
	}
	speedPowerups.clear();
	speedPowerupShapes.clear();

	for(int i = 0; i < mapPowerups.size(); i++)
	{
		mapPowerups[i].reset();
		mapPowerupShapes[i].reset();
	}
	mapPowerups.clear();
	mapPowerupShapes.clear();

	for (int i = 0; i < ammoPowerups.size(); i++)
	{
		ammoPowerups[i].reset();
		ammoPowerupShapes[i].reset();
	}
	ammoPowerups.clear();
	ammoPowerupShapes.clear();

    playerSprite.reset();
	ls::unload();
	Scene::UnLoad();
}

void Level1Scene::Update(const double& dt)
{
    if (ls::getTileAt(player->getPosition()) == ls::END) 
    {
        ofstream leaderBoardFile;
        leaderBoardFile.open("res/Leaderboard.txt");
        leaderBoardFile << to_string(timerText->GetCurrentTime()) << endl;
        Engine::ChangeScene((Scene*)&level2);
    }

    // If the player touches the weapon powerup, give the player a weapon and remove the powerup shape from the game, to prevent multiple weapons being picked up.
	// Also adds 10 seconds to the timer.
    if (!pickedUpWeapon && playerShape->getShape().getGlobalBounds().findIntersection(weaponShape->getShape().getGlobalBounds()))
	{
		timerText->ChangeTime(10.f);
		weapon->setForDelete();
    	auto newWeapon = player->addComponent<PlayerWeaponComponent>();
        playerSprite->setTextureRect(IntRect(Vector2(20, 0), Vector2(20, 30)));
		pickedUpWeapon = true;
	}
	else
	{
		// If the player touches the weapon powerup, give the player a weapon and remove the powerup shape from the game, to prevent multiple weapons being picked up.
		// Also adds 10 seconds to the timer.
		if (!pickedUpWeapon && playerShape->getShape().getGlobalBounds().findIntersection(weaponShape->getShape().getGlobalBounds())
			&& Keyboard::isKeyPressed(CONTROLS[4]))
		{
			timerText->ChangeTime(10.f);
			weapon->setForDelete();
			player->get_components<PlayerWeaponComponent>()[0]->_canFire = true;
			pickedUpWeapon = true;
		}

		// Only do collision checks if there are no speed powerups remaining on the level for efficiency.
		// Also, for all powerups, you must press the enter key to pickup the powerup, allowing the player to choose if they need the powerup
		// or if they can beat the level with a higher time score.
		if (!speedPowerups.empty())
		{
			// For each speed powerup in the level, check for collision with the player. If there is a collision:
			// - Activate the speed powerup for 10 seconds.
			// - Remove that powerup from the level, as well as deleting that entity from the list of speed powerups in the level.
			// - Subtract time as the cost of using the powerup.
			for (int i = 0; i < speedPowerups.size(); i++)
			{
				if (playerShape->getShape().getGlobalBounds().findIntersection(speedPowerupShapes[i]->getShape().getGlobalBounds())
					&& Keyboard::isKeyPressed(CONTROLS[4]))
				{
					player->get_components<PowerupManagerComponent>()[0]->ActivateSpeedPowerup();
					timerText->ChangeTime(-5.f);
					speedPowerups[i]->setForDelete();
					speedPowerups.erase(speedPowerups.begin() + i);
					speedPowerupShapes.erase(speedPowerupShapes.begin() + i);
				}
			}
		}

		// Same implementation method as the speed powerup collision check above, but for the map powerup.
		if (!mapPowerups.empty())
		{
			for (int i = 0; i < mapPowerups.size(); i++)
			{
				if (playerShape->getShape().getGlobalBounds().findIntersection(mapPowerupShapes[i]->getShape().getGlobalBounds())
					&& Keyboard::isKeyPressed(CONTROLS[4]))
				{
					player->get_components<PowerupManagerComponent>()[0]->ActivateMapPowerup();
					timerText->ChangeTime(-5.f);
					mapPowerups[i]->setForDelete();
					mapPowerups.erase(mapPowerups.begin() + i);
					mapPowerupShapes.erase(mapPowerupShapes.begin() + i);
				}
			}
		}

		// Same implementation method as the speed powerup collision check above, but for the ammo powerup.
		if (!ammoPowerups.empty())
		{
			for (int i = 0; i < ammoPowerups.size(); i++)
			{
				if (playerShape->getShape().getGlobalBounds().findIntersection(ammoPowerupShapes[i]->getShape().getGlobalBounds())
					&& Keyboard::isKeyPressed(CONTROLS[4]))
				{
					player->get_components<PlayerWeaponComponent>()[0]->AddAmmo(10);
					timerText->ChangeTime(-5.f);
					ammoPowerups[i]->setForDelete();
					ammoPowerups.erase(ammoPowerups.begin() + i);
					ammoPowerupShapes.erase(ammoPowerupShapes.begin() + i);
					ammoPowerupShapes.erase(ammoPowerupShapes.begin() + i);
				}
			}
		}

		player->get_components<PowerupManagerComponent>()[0]->IsMapPowerupActive() ? renderRange = 450.f : renderRange = 150.f;


		if ((anEnemy->getPosition() - player->getPosition()).length() >= 50)
		{
			auto relative_pos = Vector2i(player->getPosition()) - Vector2i(ls::getOffset());
			auto tile_coord = relative_pos / (int)ls::getTileSize();
			if (ls::getTile(Vector2ul(tile_coord)) != ls::WALL)
			{
				auto char_relative = anEnemy->getPosition() - ls::getOffset();
				auto char_tile = Vector2i(char_relative / ls::getTileSize());
				auto path = pathFind(char_tile, tile_coord);
				ai->setPath(path);
			}
		}


		Scene::Update(dt);

		// If the player runs out of time, end the game, player loses.
		if (timerText->GetCurrentTime() <= 0) { Engine::ChangeScene(&loseGame); }
	}
}

void Level1Scene::Render()
{

    // For each entity, check if it's within the player's render range (normally 150, 450 with map powerup). Render entities that are within this radius.
    for each(std::shared_ptr<Entity> e in ents.list)
    {
        // Leave timer entity alone, as it should always be rendered.
        if (e == timer) { continue; }
        Vector2f playerDistance = e->getPosition() - player->getPosition();
		playerDistance.x = abs(playerDistance.x); playerDistance.y = abs(playerDistance.y);
        playerDistance.length() > renderRange ? e->setVisible(false) : e->setVisible(true);
    }
    
	ls::render(Engine::GetWindow(), player->getPosition(), renderRange);
	Scene::Render();
}