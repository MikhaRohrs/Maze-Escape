#include "cmp_steering.h"

using namespace sf;

void SteeringComponent::update(double dt)
{
    if ((_player->getPosition() - _parent->getPosition()).length() < 50.f)
    {
        // If target (player) is more than 100 pixels away seek
        if (length(_parent->getPosition() - _player->getPosition()) > 100.0f) {
            auto output = _seek.getSteering();
            move(output._direction * (float)dt);
        }
    }
}


SteeringComponent::SteeringComponent(Entity* p, Entity* player)
    : _player(player), _seek(SteeringSeek(p, player, 100.0f)),
    _flee(SteeringFlee(p, player, 100.0f)), Component(p) {}

bool SteeringComponent::isValidMove(const sf::Vector2f& pos) const {
    if (pos.x < 0.0f || pos.x > Engine::GetWindow().getSize().x ||
        pos.y < 0.0f || pos.y > Engine::GetWindow().getSize().y) {
        return false;
    }
    return true;
}

void SteeringComponent::move(const sf::Vector2f& p) {
    auto new_pos = _parent->getPosition() + p;
    if (isValidMove(new_pos)) {
        _parent->setPosition(new_pos);
    }
}

void SteeringComponent::move(float x, float y) { move(Vector2f(x, y)); }