#pragma once
#include <ecm.h>
#include "../enemy_ai/steering_behaviours.h"

class SteeringComponent : public Component {
protected:
    Seek _seek;
	Entity* _player;
    bool validMove(const sf::Vector2f&) const;
public:
    void update(double) override;
    void move(const sf::Vector2f&);
    void move(float x, float y);
    void render() override { }
    explicit SteeringComponent(Entity* p, Entity* player);
    SteeringComponent() = delete;
};