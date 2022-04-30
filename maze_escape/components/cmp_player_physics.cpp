#include "ecm.h"
#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include "../game.h"

using namespace std;
using namespace sf;
using namespace Physics;

void PlayerPhysicsComponent::update(double dt)
{
    // Tracks whether a horizontal and/or a vertical directional key was pressed (Used for damping later on).
    bool pressedKeyX = false;
    bool pressedKeyY = false;

    // For each key:
    //   - Check if corresponding key was pressed (Arrow keys or WASD)
    //   - If pressed, check if current velocity in that direction exceeds the max velocity.
    //   - If linear velocity is under the max velocity, apply a impulse, scaled by the ground speed of the player.
    //   - Set pressed key bool to true (For the corresponding axis).

    // Left
    if(Keyboard::isKeyPressed(CONTROLS[2]))
    {
        if (getVelocity().x > -_maxVelocity.x) { impulse({ -static_cast<float>(dt * _speed), 0 }); pressedKeyX = true; _parent->setRotation(270.f); }
    }

    // Right
    if (Keyboard::isKeyPressed(CONTROLS[3]))
    {
        if (getVelocity().x < _maxVelocity.x) { impulse({ static_cast<float>(dt * _speed), 0 }); pressedKeyX = true; _parent->setRotation(90.f); }
    }

    // Up
    if (Keyboard::isKeyPressed(CONTROLS[0]))
    {
        if (getVelocity().y > -_maxVelocity.y) { impulse({ 0, -static_cast<float>(dt * _speed) }); pressedKeyY = true; _parent->setRotation(180.f); }
    }

    // Down
    if (Keyboard::isKeyPressed(CONTROLS[1]))
    {
        if (getVelocity().y < _maxVelocity.y) { impulse({ 0, static_cast<float>(dt * _speed) }); pressedKeyY = true; _parent->setRotation(0.f); }
    }


    // Set the rotation of the player diagonally. This is done by:
    // 1. Checking the linear velocity on the x then y axis.
    // 2. If the linear velocity for both the x and y axis are 75, including negative values, then set the rotation to face that direction.
    //
    // For example, if both the player's x and y velocities are exceeding 75 and both are positive, the player is moving
    // top-right, so set the rotation to in-between 0 (Moving up) and 90 (Moving right) degrees, which is 45 degrees (Top-right).
    if(getVelocity().x > 75)
    {
	    if(getVelocity().y > 75)
	    {
		    // Top right
            _parent->setRotation(45.f);
	    }
        else if (getVelocity().y < -75)
        {
	        // Bottom right
            _parent->setRotation(135.f);
        }
    }
    else if(getVelocity().x < -75)
    {
	    if(getVelocity().y > 75)
	    {
		    // Top left
            _parent->setRotation(315.f);
	    }
        else if(getVelocity().y < -75)
        {
	        // Bottom left
            _parent->setRotation(225.f);
        }
    }


    // Check the condition of both pressed key booleans. If one is false, it means that the player is not pressing a button
    // to move in that axis, and so should slow down in that direction and eventually stop, so dampen the velocity for that axis while not
    // affecting the other axis.
    dampen({ (pressedKeyX) ? 1.0f : 0.7f, 
				 (pressedKeyY) ? 1.0f : 0.7f});

    // Prevents speed exceeding the max velocity from traveling diagonally: checks if player's combined x and y velocities are over 200
    // (Which means it's traveling faster than it should be, and so means it's traveling diagonally). If true, change velocity clamp
    // to 200 / sqrt(2) to maintain consistent speed.
    //
    // If player speed no longer exceeds the normal max velocity of 200, set the max velocity back to 200.
    if(abs(getVelocity().x) + abs(getVelocity().y) > 200) { _maxVelocity = { _SpeedPowerupMultiplier * 141.8f, _SpeedPowerupMultiplier * 141.8f }; }
    else { _maxVelocity = { _SpeedPowerupMultiplier * 200, _SpeedPowerupMultiplier * 200 }; }

	// Clamp velocity.
	auto v = getVelocity();
	v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
	v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
	setVelocity(v);

	PhysicsComponent::update(dt);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
                                               const Vector2f& size)
    : PhysicsComponent(p, true, size), _SpeedPowerupMultiplier(1.f) {
	_size = sv2_to_bv2(size, true);
	_maxVelocity = Vector2f(200.f, 200.f);
	_speed = 66.f;
	_grounded = false;
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(true);
	//Bullet items have higher-res collision detection
	_body->SetBullet(true);
	// Minor friction for more rigid/less floaty controls
	setFriction(0.2f);
}
