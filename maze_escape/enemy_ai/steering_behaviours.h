#pragma once
#include "engine.h"

struct SteeringOutput
{
	// Direction of travel.
	sf::Vector2f direction;
	// Rotation of travel.
	float rotation;
};


class SteeringBehaviour {
public:
	virtual ~SteeringBehaviour() = default;

	// Gets the output from a steering behaviour.
	virtual SteeringOutput getSteering() const noexcept = 0;
};


class Seek : public SteeringBehaviour {
private:
	Entity* _character;
	Entity* _target;
	float _maxSpeed;
public:
	Seek() = delete;
	Seek(Entity* character, Entity* target, float maxSpeed)
		: _character(character), _target(target), _maxSpeed(maxSpeed) { }
	SteeringOutput getSteering() const noexcept;
};