#include "ecm.h"
#include "cmp_weapon.h"
#include "engine.h"
#include "cmp_projectile.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
using namespace std;
using namespace sf;

void PlayerWeaponComponent::update(double dt)
{
    // If the spacebar is fired, decrement ammo and generate a new projectile. 1 second cooldown per attack.
    if(Keyboard::isKeyPressed(Keyboard::Space) && _cooldown <= 0)
    {
        _cooldown = 1.f;
        Attack();
        _ammo--;
    }
    // Delta time decrements the cooldown.
    _cooldown -= dt;
}

// Launch a projectile in a given direction
void PlayerWeaponComponent::Attack()
{
    // Create projectile entity
    auto projectile = _parent->scene->makeEntity();
    Vector2f facingDirection = { 0.f, 0.f };

    // Determine it's facing direction from the player's rotation, used to determine which way the bullet will fire.
    // Each rotation is checked directly by a flat float value, but should return no errors as player movement will set
    // player's rotation to one of these set values.

    switch (static_cast<int>(_parent->getRotation()))
    {
    // Up
    case 0:   facingDirection = { 0.f, 20.f };    break;

    // Up-Right
    case 45:  facingDirection = { 10.f, -10.f };  break;

    // Right
    case 90:  facingDirection = { 20.f, 0.f };    break;

    // Down-Right
    case 135: facingDirection = { 10.f, 10.f };   break;

    // Down
    case 180: facingDirection = { 0.f, -20.f };   break;

    // Down-Left
    case 225: facingDirection = { -10.f, 10.f };  break;

    // Left
    case 270: facingDirection = { -20.f, 0.f };   break;

    // Up-Left
    case 315: facingDirection = { -10.f, -10.f }; break;

    // Should never reach default because of set rotational values, but if an invalid rotation is found, default the bullet to go up.
    default:  facingDirection = { 0.f, 20.f };    break;
    }
    
    // Projectile spawns infront of the player, determined by the direction the player is facing.
    projectile->setPosition(_parent->getPosition() + facingDirection);

    // Create shape component of the projectile, setup visuals of the projectile.
    auto shape = projectile->addComponent<ShapeComponent>();
    shape->setShape<sf::RectangleShape>(sf::Vector2f(4.f, 4.f));
    shape->getShape().setFillColor(Color::Blue);
    shape->getShape().setOutlineColor(Color::Black);
    shape->getShape().setOutlineThickness(2);
    shape->getShape().setOrigin(Vector2f(2.f, 2.f));

    // Projectile component handles projectile collisions and lifetime.
    projectile->addComponent<ProjectileComponent>();

    // Add physics component to the projectile.
    auto physics = projectile->addComponent<PhysicsComponent>(true, Vector2f(4.f, 4.f));
    physics->setRestitution(1);
    physics->setFriction(0);

    // Provide impulse force for the projectile in the direction of fire.
    physics->impulse(facingDirection * Vector2f{.8f, .8f});
}

PlayerWeaponComponent::PlayerWeaponComponent(Entity* p) : Component(p), _ammo(10), _cooldown(0.f) {}