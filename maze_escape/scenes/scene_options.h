#pragma once

#include "scene_menu.h"

class Options : public MenuScene
{
public:
	Options() = default;
	~Options() override = default;

	void Load() override;

	void Update(const double& dt) override;
};