#include "ecm.h"
#include "cmp_weapon.h"
#include "engine.h"
#include "cmp_projectile.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
using namespace std;
using namespace sf;

void PlayerWeaponComponent::update(double dt)
{
    if(Keyboard::isKeyPressed(Keyboard::Space) && _cooldown <= 0)
    {
        _cooldown = 1.f;
        Attack();
        _ammo--;
    }
    _cooldown -= dt;
}

void PlayerWeaponComponent::Attack()
{
    auto projectile = _parent->scene->makeEntity();
    Vector2f facingDirection = { 0.f, 0.f };

    // Up
    if (_parent->getRotation() == 0.f)       { facingDirection = { 0.f, 5.f };      }
    // Up-Right
    else if (_parent->getRotation() == 45.f) { facingDirection = { 2.5f, -2.5f };    }

    // Right
    else if (_parent->getRotation() == 90.f)    { facingDirection = { 5.f, 0.f };     }
    // Down-Right
    else if (_parent->getRotation() == 135.f) { facingDirection = { 2.5f, 2.5f };  }

    // Down
    else if (_parent->getRotation() == 180.f) { facingDirection = { 0.f, -5.f };    }
    //Down-Left
    else if (_parent->getRotation() == 225.f) { facingDirection = { -2.5f, 2.5f }; }

    // Left
    else if (_parent->getRotation() == 270.f) { facingDirection = { -5.f, 0.f };    }
    // Up-Left
    else if (_parent->getRotation() == 315.f) { facingDirection = { -2.5f, -2.5f };  }
    

    projectile->setPosition(_parent->getPosition() + facingDirection);

    auto shape = projectile->addComponent<ShapeComponent>();
    shape->setShape<sf::RectangleShape>(sf::Vector2f(6.f, 6.f));
    shape->getShape().setFillColor(Color::Blue);
    shape->getShape().setOutlineColor(Color::Black);
    shape->getShape().setOutlineThickness(2);
    shape->getShape().setOrigin(Vector2f(3.f, 3.f));

    projectile->addComponent<ProjectileComponent>();


    auto physics = projectile->addComponent<PhysicsComponent>(true, Vector2f(6.f, 6.f));
    physics->setRestitution(1);
    physics->setFriction(0);
    physics->impulse(facingDirection * Vector2f{4, 4});
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