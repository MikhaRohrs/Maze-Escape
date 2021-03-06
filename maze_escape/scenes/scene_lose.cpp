#include "scene_lose.h"
#include "../components/cmp_text.h"
#include "../game.h"

using namespace std;
using namespace sf;

float timer;
void LoseScene::Load()
{
    timer = 5.f;
	auto txt = makeEntity();
	auto text = txt->addComponent<TextComponent>("You lose.\nReturning to the main menu in 5 seconds...");
    setLoaded(true);
}

void LoseScene::Update(const double& dt)
{
    timer -= dt;
    if (timer <= 0) 
    {
        Engine::ChangeScene(&menu);
    }
    Scene::Update(dt);
}