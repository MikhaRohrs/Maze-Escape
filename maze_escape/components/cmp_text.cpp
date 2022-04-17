#include "ecm.h"
#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::update(double dt) {}

void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const std::string& str)
    : Component(p), _string(str) {
  _text.setString(_string);
  _font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
  _text.setFont(*_font);
}

void TextComponent::SetText(const std::string& str) {
  _string = str;
  _text.setString(_string);
}

// Get the text internal object of a text component. This is chosen over having functions for standard text modification operations for text components
// (Such as .move(), .setPosition(), .setFillColor() ) as this function concatenates all this into two function calls: one for the text getter
// and one for the sfml function.
sf::Text& TextComponent::GetTextObject()
{
	return _text;
}


