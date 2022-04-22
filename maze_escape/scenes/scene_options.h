#pragma once

#include "engine.h"

class Options : public Scene
{
public:
	Options() = default;
	~Options() override = default;

	void Load() override;

	void Update(const double& dt) override;
};