#include "ecm.h"
#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::update(double dt)
{
    _text.setPosition(_parent->getPosition());
}

void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const std::string& str) : Component(p), _string(str) {
  _text.setString(_string);
  _font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
  _text.setFont(*_font);
}

void TextComponent::SetText(const std::string& str)
{
  _string = str;
  _text.setString(_string);
}

void TextComponent::setPosition(sf::Vector2f position)
{
    _text.setPosition(position);
}

void TextComponent::setOrigin(sf::Vector2f origin)
{
    _text.setOrigin(origin);
}