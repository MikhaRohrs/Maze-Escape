#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
LoseScene loseGame;
LeaderBoard leaderBoard;
Options optionScene;

int main() {
    Engine::Start(GAME_WIDTH[CURRENT_RES], GAME_HEIGHT[CURRENT_RES], "Maze Escape", &menu);
}