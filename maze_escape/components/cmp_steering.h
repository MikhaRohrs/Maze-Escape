#pragma once
#include <ecm.h>
#include "../enemy_ai/steering_behaviours.h"

class SteeringComponent : public Component
{
protected:
	SteeringSeek _seek;
	SteeringFlee _flee;
	Entity* _player;
	bool _nearPlayer;

	bool isValidMove(const sf::Vector2f&) const;

public:
	void update(double dt) override;
	void move(const sf::Vector2f&);
	void move(float x, float y);
	void render() override {}
	explicit SteeringComponent(Entity* p, Entity* player);
	SteeringComponent() = delete;
};