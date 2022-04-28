#include "scene_options.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "SFML/Window/Event.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#include "SFML/Window/VideoMode.hpp"

using namespace std;
using namespace sf;

string soundState = "ON";

void Options::Load()
{
	cout << "Options load\n";
	{
		constexpr int numOfOptions = 3;
		const string optionTexts[numOfOptions] = { "Remap controls", "Toggle sound: " + soundState, "Back" };
		SelectedOption = 0;

		const auto title = makeEntity();
		auto titleText = title->addComponent<TextComponent>("Maze Escape/Options");

		float offset = 0.0f;
		for (int i = 0; i < numOfOptions; i++)
		{
			MenuOptions.push_back(makeEntity());
			Texts.push_back(MenuOptions[i]->addComponent<TextComponent>(optionTexts[i]));
			if (i != 0)
			{
				Texts[i]->ChangeColor(Grey);
			}
			offset -= 50.0f;
			MenuOptions[i]->setPosition(Vector2f(70.0f, GAME_HEIGHT - (GAME_HEIGHT + offset)));
		}
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
		case 0: // Remap controls
			Engine::ChangeScene(&controlOptions);
			break;
		case 1: // Toggle sound
			soundEnabled = !soundEnabled;
			if (soundEnabled)
			{
				soundState = "ON";
			}
			else
			{
				soundState = "OFF";
			}
			Engine::ChangeScene(&optionScene);
			break;
		case 2: // Main menu
			Engine::ChangeScene(&menu);
			break;
		default:
			break;
		}
	}
	MenuScene::Update(dt);
}