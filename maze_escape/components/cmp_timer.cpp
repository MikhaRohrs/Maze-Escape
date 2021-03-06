#include "ecm.h"
#include "cmp_timer.h"
#include <system_renderer.h>
#include <system_resources.h>

void TimerComponent::update(double dt)
{
    // Decrement timer by delta time.
    _currentTime -= dt;

    // Timer will flash red if there are 10 seconds or less remaining.
    // Otherwise, timer is black as normal. (Else statement needed if the player has <10 seconds left then gains additional time
    // greater than 10 seconds
    if (_currentTime <= 11) { LowTimeFlash(); }
	else {_text->setFillColor(sf::Color::Black);}

    // Cosmetic change where the text is shifted very slightly to the right when it reaches single digit numbers (As the number text is left-aligned).
    // Else statement moves it back when it has double digits, needed when the timer reaches <10 seconds, the text moves to the right slightly to keep it centred,
    // then if time is gained with result > 10 seconds, re-centre it.
    if (_currentTime <= 10) { _text->setPosition(sf::Vector2f(14, 0)); }
    else {_text->setPosition({ 5, 0 }); }

    // Each frame update the timer text to show the remaining time, cast to an integer without rounding.
    _text->setString(std::to_string(static_cast<int>(_currentTime)));
}


void TimerComponent::render() { Renderer::queue(_text.get()); }

TimerComponent::TimerComponent(Entity* const p)
    : Component(p), _currentTime(20.0f), _text(std::make_shared<sf::Text>())
{
	_font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
    _text->setFont(*_font);
    _text->setString(std::to_string(static_cast<int>(_currentTime)));
}

// If time is under 11 seconds (In this case, it is treated as true starting from 10 seconds remaining on the visible, in-game clock), make the timer flash
// red to indicate that the player is running out of time.
void TimerComponent::LowTimeFlash() const
{
    bool evenNumber = (static_cast<int>(_currentTime) % 2 == 0) ? true : false;
    if (evenNumber) { _text->setFillColor(sf::Color::Red); }
    else { _text->setFillColor(sf::Color::Black); }
}

// Get the text internal object of a text component. This is chosen over having functions for standard text modification operations for text components
// (Such as .move(), .setPosition(), .setFillColor() ) as this function concatenates all this into two function calls: one for the text getter
// and one for the sfml function.
sf::Text& TimerComponent::GetTextObject() const
{
    return *_text;
}

float TimerComponent::GetCurrentTime() const { return _currentTime; }

// Add or subtract time (in seconds) onto the timer. Positive values = increase in time, negative values = decrease in time.
void TimerComponent::ChangeTime(float timeChange)
{
    _currentTime += timeChange;
}