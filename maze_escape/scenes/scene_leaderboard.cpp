#include "scene_leaderboard.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "SFML/Window/Event.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void LeaderBoard::Load()
{
	cout << "Leaderboard load\n";
	{
		constexpr int numOfOptions = 1;
		const string optionTexts[numOfOptions] = { "Back" };
		SelectedOption = 0;

		const auto title = makeEntity();
		auto titleText = title->addComponent<TextComponent>("Leaderboard");

		MenuOptions.push_back(makeEntity());
		Texts.push_back(MenuOptions[0]->addComponent<TextComponent>(optionTexts[0]));

		MenuOptions[0]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.47f));
	}
	setLoaded(true);
}

void LeaderBoard::Update(const double& dt)
{
	if (Keyboard::isKeyPressed(CONTROLS[4]) && CoolDown <= 0)
	{
		CoolDown = 0.2f;
		MenuOptions.clear();
		Texts.clear();
		Engine::ChangeScene(&menu);
	}
	MenuScene::Update(dt);
}