#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "SFML/Window/VideoMode.hpp"
#include <Windows.h>

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
	// TODO: Uncomment when ready to release, in order to hide the command console screen
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	gameWidth = sf::VideoMode::getDesktopMode().width;
	gameHeight = sf::VideoMode::getDesktopMode().height;

    //Engine::Start(GAME_WIDTH[CURRENT_RES], GAME_HEIGHT[CURRENT_RES], "Maze Escape", &menu);
    Engine::Start(gameWidth, gameHeight, "Maze Escape", &menu);
}