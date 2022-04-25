#include "ecm.h"
#include "cmp_weapon.h"
#include "engine.h"
#include "cmp_enemy_turret.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
using namespace std;
using namespace sf;

void PlayerWeaponComponent::update(double dt)
{
    if(Keyboard::isKeyPressed(Keyboard::Space) && _cooldown <= 0)
    {
        _cooldown = 2.f;
        Attack();
        _ammo--;
    }
    _cooldown -= dt;
}

void PlayerWeaponComponent::Attack()
{
    auto projectile = _parent->scene->makeEntity();
    projectile->setPosition(_parent->getPosition() + Vector2f(0.f, 4.f));
    // Add component to move projectile, expire after time delay/contact with wall tile, kill enemies on contact.

    auto shape = projectile->addComponent<ShapeComponent>();
    shape->setShape<sf::RectangleShape>(sf::Vector2f(6.f, 6.f));
    shape->getShape().setFillColor(Color::Blue);
    shape->getShape().setOutlineColor(Color::Black);
    shape->getShape().setOutlineThickness(2);
    shape->getShape().setOrigin(Vector2f(3.f, 3.f));

    auto physics = projectile->addComponent<PhysicsComponent>(true, Vector2f(6.f, 6.f));
    physics->setRestitution(.0f);
    physics->setFriction(.005f);
    physics->impulse(sf::rotate(Vector2f(0, 15.f), -_parent->getRotation()));
}

PlayerWeaponComponent::PlayerWeaponComponent(Entity* p) : Component(p), _ammo(10) {}



/*
void EnemyTurretComponent::fire() const {
    auto bullet = _parent->scene->makeEntity();
    bullet->setPosition(_parent->getPosition());
    bullet->addComponent<HurtComponent>();
    bullet->addComponent<BulletComponent>();
    auto s = bullet->addComponent<ShapeComponent>();

    s->setShape<sf::CircleShape>(8.f);
    s->getShape().setFillColor(Color::Red);
    s->getShape().setOrigin(Vector2f(8.f, 8.f));
    auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(8.f, 8.f));
    p->setRestitution(.4f);
    p->setFriction(.005f);
    p->impulse(sf::rotate(Vector2f(0, 15.f), -_parent->getRotation()));
}

EnemyTurretComponent::EnemyTurretComponent(Entity* p)
    : Component(p), _firetime(2.f) {}
    */