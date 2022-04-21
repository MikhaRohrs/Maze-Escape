#pragma once

#include "engine.h"

class LeaderBoard : public Scene
{
public:
	LeaderBoard() = default;
	~LeaderBoard() override = default;

	void Load() override;

	void Update(const double& dt) override;
};