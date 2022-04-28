#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "SFML/Window/VideoMode.hpp"

using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
LoseScene loseGame;
LeaderBoard leaderBoard;
Options optionScene;
ControlOptions controlOptions;
LevelSelect levelSelect;

int main()
{
	const auto desktopWidth = sf::VideoMode::getDesktopMode().width;
	const auto desktopHeight = sf::VideoMode::getDesktopMode().height;

    //Engine::Start(GAME_WIDTH[CURRENT_RES], GAME_HEIGHT[CURRENT_RES], "Maze Escape", &menu);
    Engine::Start(desktopWidth, desktopHeight, "Maze Escape", &menu);
}