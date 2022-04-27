#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class ProjectileComponent : public Component {
protected:
  float _lifetime;

public:
  void update(double dt) override;
  void render() override {}
  explicit ProjectileComponent(Entity* p);
  ProjectileComponent() = delete;
};
