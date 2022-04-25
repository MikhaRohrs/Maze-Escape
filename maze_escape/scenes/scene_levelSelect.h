#pragma once

#include "scene_menu.h"

class LevelSelect : public MenuScene
{
public:
	LevelSelect() = default;
	~LevelSelect() override = default;

	void Load() override;

	void Update(const double& dt) override;
};