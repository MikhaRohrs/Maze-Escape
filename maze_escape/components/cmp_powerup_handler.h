#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class PowerupManagerComponent : public Component {
protected:
	bool _speedPowerupActive;
	bool _mapPowerupActive;

	float _speedPowerupCurrentDuration;
	float _mapPowerupCurrentDuration;

public:
	void update(double dt) override;
	void render() override {}
	explicit PowerupManagerComponent(Entity* p);
	PowerupManagerComponent() = delete;

	void ActivateSpeedPowerup();
	void ActivateMapPowerup();

	bool IsSpeedPowerupActive() const;
	bool IsMapPowerupActive() const;
};
