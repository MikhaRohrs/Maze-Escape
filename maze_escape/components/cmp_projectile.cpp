#include "ecm.h"
#include "cmp_projectile.h"
using namespace std;
using namespace sf;

void ProjectileComponent::update(double dt) {
  _lifetime -= dt;
  if (_lifetime <= 0.f) {
    _parent->setForDelete();
  }
}

ProjectileComponent::ProjectileComponent(Entity* p)
    : Component(p), _lifetime(2.f) {}
