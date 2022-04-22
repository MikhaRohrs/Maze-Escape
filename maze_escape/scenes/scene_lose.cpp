#include "scene_lose.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void LoseScene::Load() {
    cout << "Menu Load \n";
    {
        auto txt = makeEntity();
        auto t = txt->addComponent<TextComponent>(
            "You Lose.\n\nPress Space to return to the main menu.");
    }
    setLoaded(true);
}

void LoseScene::Update(const double& dt)
{
    if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
        Engine::ChangeScene(&menu);
    }

    Scene::Update(dt);
}