
#pragma once

#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_level3.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_leaderboard.h"
#include "scenes/scene_options.h"

extern MenuScene menu;
extern Level1Scene level1;
extern Level2Scene level2;
extern Level3Scene level3;
extern LeaderBoard leaderBoard;
extern Options optionScene;

constexpr uint16_t GAME_WIDTH[4] = { 160, 640, 1280, 1920 };
constexpr uint16_t GAME_HEIGHT[4] = { 192, 480, 720, 1080 };
constexpr int CURRENT_RES = 2;