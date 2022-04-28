#pragma once

#include "scene_menu.h"

class ControlOptions : public MenuScene
{
public:
	ControlOptions() = default;
	~ControlOptions() override = default;

	static std::string KeycodeToString(sf::Keyboard::Key& key);

	void Load() override;

	void Update(const double& dt) override;

private:
	void ControlOptions::ReplaceControl(int controlNum, const double& dt);
};