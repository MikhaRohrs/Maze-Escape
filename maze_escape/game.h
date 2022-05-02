
#pragma once

#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_level3.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_leaderboard.h"
#include "scenes/scene_levelSelect.h"
#include "scenes/scene_lose.h"
#include "scenes/scene_options.h"
#include "scenes/scene_options_controls.h"
#include "SFML/Window/Keyboard.hpp"

extern MenuScene menu;
extern Level1Scene level1;
extern Level2Scene level2;
extern Level3Scene level3;
extern LoseScene loseGame;
extern LeaderBoard leaderBoard;
extern Options optionScene;
extern LevelSelect levelSelect;
extern ControlOptions controlOptions;

inline uint16_t gameWidth;
inline uint16_t gameHeight;

inline bool soundEnabled = true;

inline sf::Keyboard::Key CONTROLS[5] = 
{
	sf::Keyboard::W, // 0 - UP
	sf::Keyboard::S, // 1 - DOWN
	sf::Keyboard::A, // 2 - LEFT
	sf::Keyboard::D, // 3 - RIGHT
	sf::Keyboard::Enter // 4 - INTERACT or SELECT
};

inline int selectedLevel = 1;

