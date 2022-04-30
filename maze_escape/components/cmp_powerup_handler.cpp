#include "ecm.h"
#include "cmp_powerup_handler.h"
#include "cmp_player_physics.h"
#include "engine.h"

using namespace std;
using namespace sf;

// Manages the effects of collected powerups, their durations, and expiring their effects.

void PowerupManagerComponent::update(double dt)
{
	// Decrement duration by delta time.
	_speedPowerupCurrentDuration -= dt;
	_mapPowerupCurrentDuration -= dt;

	// Check if the duration of the speed buff is under 0, in which case, end the effect (set speed multiplier to 1x), and set the active flag to false.
	// Active flag used to prevent this if statement executing every frame when duration is negative so that it doesn't modify the speed multiplier per frame.
	if ( _speedPowerupActive && _speedPowerupCurrentDuration <= 0.f)
	{
		_speedPowerupActive = false;
		_parent->get_components<PlayerPhysicsComponent>()[0]->_SpeedPowerupMultiplier = 1.f;
	}

	// Same implementation method as speed powerup but changes the render range variable instead.
	if (_mapPowerupActive && _mapPowerupCurrentDuration <= 0.f)
	{
		_mapPowerupActive = false;
	}
}


// Initially, powerup active effects are set to false and durations set to 0.
PowerupManagerComponent::PowerupManagerComponent(Entity* p) : Component(p), _speedPowerupActive(false), _mapPowerupActive(false),
_speedPowerupCurrentDuration(0.f), _mapPowerupCurrentDuration(0.f) {}

// Activate the speed powerup:
// - Set the active flag to true.
// - Add 10 seconds to the duration (Allows stacking of powerups without losing out on duration).
// - Apply speed buff by setting the speed multiplier in the player physics component to 2x, which doubles the player's max velocity.
void PowerupManagerComponent::ActivateSpeedPowerup()
{
	_speedPowerupCurrentDuration += 10.f;
	_speedPowerupActive = true;
	_parent->get_components<PlayerPhysicsComponent>()[0]->_SpeedPowerupMultiplier = 2.f;
}

// Activate the speed powerup:
// - Set active flag to true;
// - Add 10 seconds to the duration (Allows stacking of powerups without losing out on duration).
// - Apply the map buff (Retrieved from the bool getter), which triples the render / sight radius of the player.
void PowerupManagerComponent::ActivateMapPowerup()
{
	_mapPowerupCurrentDuration += 10.f;
	_mapPowerupActive = true;
}

// Powerup active getters
bool PowerupManagerComponent::IsMapPowerupActive()   const { return _mapPowerupActive;   }
bool PowerupManagerComponent::IsSpeedPowerupActive() const { return _speedPowerupActive; }

