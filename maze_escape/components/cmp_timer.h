#pragma once

#include <ecm.h>
#include "SFML/Graphics/Text.hpp"

class TimerComponent : public Component {
public:
	TimerComponent() = delete;
	explicit TimerComponent(Entity* p);

	void update(double dt) override;
	void render() override;

	~TimerComponent() override = default;

	void LowTimeFlash() const;
	sf::Text& GetTextObject() const;
	float GetCurrentTime() const;
	void ChangeTime(float timeChange);

protected:
	float _currentTime;
	std::shared_ptr<sf::Text> _text;
	std::shared_ptr<sf::Font> _font;
};
