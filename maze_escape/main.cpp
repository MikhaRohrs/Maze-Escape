#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_lose.h"

using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
LoseScene loseGame;


int main() {
    Engine::Start(1280, 720, "Platformer", &menu);
}