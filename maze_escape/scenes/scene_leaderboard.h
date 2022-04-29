#pragma once

#include "scene_menu.h"

class LeaderBoard : public MenuScene
{
public:
	LeaderBoard() = default;
	~LeaderBoard() override = default;

	void Load() override;

	void Update(const double& dt) override;

private:
	void ReadLeaderboardFile();
	void ResetLeaderboardFile();

	std::vector<std::string> ScoreList;
};