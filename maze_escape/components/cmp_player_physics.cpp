#include "ecm.h"
#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;
using namespace Physics;

void PlayerPhysicsComponent::update(double dt)
{
    // Tracks whether a horizontal and/or a vertical directional key was pressed (Used for damping later on).
    bool pressedKeyX = false;
    bool pressedKeyY = false;

    // For each key:
    //   - Check if corresponding key was pressed (Arrow keys and WASD)
    //   - If pressed, check if current velocity in that direction exceeds the max velocity.
    //   - If linear velocity is under the max velocity, apply a impulse, scaled by the ground speed of the player.
    //   - Set pressed key bool to true (For the corresponding axis).
    if(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
    {
        if (getVelocity().x > -_maxVelocity.x) { impulse({ -static_cast<float>(dt * _groundspeed), 0 }); pressedKeyX = true; }
    }

    if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
    {
    	if (getVelocity().x < _maxVelocity.x) { impulse({ static_cast<float>(dt * _groundspeed), 0 }); pressedKeyX = true; }
    }

    if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
    {
        if (getVelocity().y > -_maxVelocity.y) { impulse({ 0, -static_cast<float>(dt * _groundspeed) }); pressedKeyY = true; }
    }

    if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
    {
        if (getVelocity().y < _maxVelocity.y) { impulse({ 0, static_cast<float>(dt * _groundspeed) }); pressedKeyY = true; }
    }

    // Check the condition of both pressed key booleans. If one is false, it means that the player is not pressing a button
    // to move in that axis, and so should slow down/stop moving in that direction, so dampen the velocity for that axis, while not
    // affecting the other axis.
    dampen({ (pressedKeyX) ? 1.0f : 0.7f, 
				 (pressedKeyY) ? 1.0f : 0.7f});

    // Prevents speed exceeding the max velocity from traveling diagonally: checks if player's combined x and y velocities are over 200
    // (Which means it's traveling faster than it should be, and so means it's traveling diagonally). If true, change velocity clamp
    // to 200 / sqrt(2) to maintain consistent speed.
    //
    // If player speed no longer exceeds the normal max velocity of 200, set the max velocity back to 200.
    if(abs(getVelocity().x) + abs(getVelocity().y) > 200) { _maxVelocity = { 141.8f, 141.8f }; }
    else { _maxVelocity = { 200, 200 }; }

  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
  setVelocity(v);

  PhysicsComponent::update(dt);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
                                               const Vector2f& size)
    : PhysicsComponent(p, true, size) {
  _size = sv2_to_bv2(size, true);
  _maxVelocity = Vector2f(200.f, 200.f);
  _groundspeed = 66.f;
  _grounded = false;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);

    // Minor friction for more rigid/less floaty controls
  setFriction(0.2f);
    // Top down game, so disable gravity.
  _body->SetGravityScale(0);
}
