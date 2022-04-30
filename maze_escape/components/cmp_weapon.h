#pragma once
#include <ecm.h>

class PlayerWeaponComponent : public Component {
protected:
	void Attack() const;
	int _ammo;
	float _cooldown = 1.f;

public:
	void update(double dt) override;
	void render() override {}
	explicit PlayerWeaponComponent(Entity* p);
	PlayerWeaponComponent() = delete;
};
