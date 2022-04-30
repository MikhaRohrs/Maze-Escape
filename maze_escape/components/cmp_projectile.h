#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class ProjectileComponent : public PhysicsComponent {
protected:
  float _lifetime;
  std::shared_ptr<Entity> _player;
  std::shared_ptr<Entity> _timer;

public:
  void update(double dt) override;
  void render() override {}
  explicit ProjectileComponent(Entity* p);
  ProjectileComponent() = delete;
};
