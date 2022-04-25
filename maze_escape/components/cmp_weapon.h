#pragma once
#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class PlayerWeaponComponent : public Component {
protected:
	void Attack();
	int _ammo;
	float _cooldown = 2.f;

public:
	void update(double dt) override;
	void render() override {}
	explicit PlayerWeaponComponent(Entity* p);
	PlayerWeaponComponent() = delete;
};
