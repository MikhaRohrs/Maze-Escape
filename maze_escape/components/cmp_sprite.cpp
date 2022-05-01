#include "ecm.h"
#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace std;


void SpriteComponent::setTexture(std::shared_ptr<sf::Texture> tex)
{
  _texture = tex;
  _sprite->setTexture(*_texture);
}

void SpriteComponent::setTextureRect(sf::IntRect rect)
{
    _sprite->setTextureRect(rect);
}

void SpriteComponent::setOrigin(sf::Vector2f origin)
{
    _sprite->setOrigin(origin);
}

SpriteComponent::SpriteComponent(Entity* p)
    : ::Component(p), _sprite(make_shared<sf::Sprite>()) {}

void SpriteComponent::update(double dt)
{
  _sprite->setPosition(_parent->getPosition());
  _sprite->setRotation(sf::degrees(_parent->getRotation()));
}

void SpriteComponent::render() { Renderer::queue(_sprite.get()); }

void ShapeComponent::update(double dt)
{
  _shape->setPosition(_parent->getPosition());
  _shape->setRotation(sf::degrees(_parent->getRotation()));
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p)
    : Component(p), _shape(make_shared<sf::CircleShape>()) {}

sf::Sprite& SpriteComponent::getSprite() const { return *_sprite; }
