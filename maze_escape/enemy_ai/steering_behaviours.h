#pragma once
#include "engine.h"

struct Steering
{
	float _rotation;
	sf::Vector2f _direction;
};

class SteeringBehaviours
{
public:
	virtual Steering getSteering() const noexcept = 0;

	virtual ~SteeringBehaviours() = default;
};


class SteeringSeek : public SteeringBehaviours
{
private:
	Entity* _character;
	Entity* _targetEntity;

	float _maxSpeed;

public:
	SteeringSeek() = delete;
	SteeringSeek(Entity* character, Entity* target, float maxSpeed) : _character(character), _targetEntity(target), _maxSpeed(maxSpeed) {}
	Steering getSteering() const noexcept;
};

class SteeringFlee : public SteeringBehaviours {
private:
	Entity* _character;
	Entity* _target;
	float _maxSpeed;
public:
	SteeringFlee() = delete;
	SteeringFlee(Entity* character, Entity* target, float maxSpeed)
		: _character(character), _target(target), _maxSpeed(maxSpeed) { }
	Steering getSteering() const noexcept;
};