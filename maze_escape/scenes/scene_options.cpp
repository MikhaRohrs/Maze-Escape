#include "scene_options.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "SFML/Window/Event.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void Options::Load()
{
	cout << "Options load\n";
	{
		constexpr int numOfOptions = 4;
		const string optionTexts[numOfOptions] = { "Screen resolution", "Remap controls", "Toggle sound", "Back" };
		SelectedOption = 0;

		const auto title = makeEntity();
		auto titleText = title->addComponent<TextComponent>("Options");

		for (int i = 0; i < numOfOptions; i++)
		{
			MenuOptions.push_back(makeEntity());
			Texts.push_back(MenuOptions[i]->addComponent<TextComponent>(optionTexts[i]));
			if (i != 0)
			{
				Texts[i]->ChangeColor(Grey);
			}
		}
		MenuOptions[0]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.1f));
		MenuOptions[1]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.2f));
		MenuOptions[2]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.32f));
		MenuOptions[3]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.47f));
	}
	setLoaded(true);
}

void Options::Update(const double& dt)
{
	if (Keyboard::isKeyPressed(CONTROLS[0]) && CoolDown <= 0)
	{
		CoolDown = 0.2f;
		if (SelectedOption - 1 >= 0)
		{
			Texts[SelectedOption]->ChangeColor(Grey);
			SelectedOption--;
			Texts[SelectedOption]->ChangeColor(Color::White);
		}
	}
	if (Keyboard::isKeyPressed(CONTROLS[1]) && CoolDown <= 0)
	{
		CoolDown = 0.2f;
		if (SelectedOption + 1 < MenuOptions.size())
		{
			Texts[SelectedOption]->ChangeColor(Grey);
			SelectedOption++;
			Texts[SelectedOption]->ChangeColor(Color::White);
		}
	}
	if (Keyboard::isKeyPressed(CONTROLS[4]) && CoolDown <= 0)
	{
		CoolDown = 0.2f;
		MenuOptions.clear();
		Texts.clear();
		switch (SelectedOption)
		{
		case 0: // Screen resolution
			break;
		case 1: // Remap controls
			Engine::ChangeScene(&controlOptions);
			break;
		case 2: // Toggle sound
			break;
		case 3: // Main menu
			Engine::ChangeScene(&menu);
			break;
		default:
			break;
		}
	}
	MenuScene::Update(dt);
}