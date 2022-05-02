#include "cmp_pathfinding.h"
#include "../enemy_ai/aStarPathfinding.h"
#include <LevelSystem.h>

using namespace sf;
using namespace std;

void PathfindingComponent::update(double dt) {
    _elapsed += dt;
    if (_elapsed >= 0.5) {
        _elapsed = 0.0;
        if (_index < _path.size()) {
            float new_x = ls::getOffset().x + _path[_index].x * ls::getTileSize();
            float new_y = ls::getOffset().y + _path[_index].y * ls::getTileSize();
            _parent->setPosition(Vector2f(new_x, new_y));
            ++_index;
        }
    }
}

PathfindingComponent::PathfindingComponent(Entity* p) : Component(p) {}

void PathfindingComponent::setPath(std::vector<sf::Vector2i>& path) {
    _index = 0;
    _path = path;
}