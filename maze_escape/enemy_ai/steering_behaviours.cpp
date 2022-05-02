#include "steering_behaviours.h"

using namespace sf;

Steering SteeringSeek::getSteering() const noexcept
{
	Steering steering;
	steering._direction = normalize(_targetEntity->getPosition() - _character->getPosition()) * _maxSpeed;
	steering._rotation = 0.f;
	return steering;
}
