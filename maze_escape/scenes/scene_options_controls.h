#pragma once

#include "scene_menu.h"

class ControlOptions : public MenuScene
{
public:
	ControlOptions() = default;
	~ControlOptions() override = default;

	void Load() override;

	void Update(const double& dt) override;
};