#pragma once

#include <ecm.h>
#include "SFML/Graphics/Text.hpp"

class TimerComponent : public Component {
public:
	TimerComponent() = delete;
	TimerComponent(Entity* p, const float startTime);

	void update(double dt) override;
	void render() override;

	~TimerComponent() override = default;

	void LowTimeFlash();
	sf::Text& GetTextObject();
	float GetCurrentTime();

protected:
	float _currentTime;
	sf::Text _text;
	std::shared_ptr<sf::Font> _font;
};
