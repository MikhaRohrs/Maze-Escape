#include "scene_leaderboard.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "SFML/Window/Event.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace sf;

void LeaderBoard::Load()
{
	cout << "Level select load\n";
	{
		constexpr int numOfOptions = 1;
		const string optionTexts[numOfOptions] = { "Back" };
		selectedOption = 0;

		const auto title = makeEntity();
		auto titleText = title->addComponent<TextComponent>("Leaderboard");

		options.push_back(makeEntity());
		texts.push_back(options[0]->addComponent<TextComponent>(optionTexts[0]));

		options[0]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.47f));
	}
	setLoaded(true);
}

void LeaderBoard::Update(const double& dt)
{
	this_thread::sleep_for(chrono::milliseconds(100));
	
	if (Keyboard::isKeyPressed(CONTROLS[4]))
	{
		options.clear();
		texts.clear();
		Engine::ChangeScene(&menu);
	}
	MenuScene::Update(dt);
}