#pragma once

#include "engine.h"

class LoseScene : public Scene {
public:
	LoseScene() = default;
	~LoseScene() override = default;

	void Load() override;

	void Update(const double& dt) override;
};
