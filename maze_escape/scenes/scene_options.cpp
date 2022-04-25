#include "scene_options.h"
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

void Options::Load()
{
	cout << "Level select load\n";
	{
		constexpr int numOfOptions = 4;
		const string optionTexts[numOfOptions] = { "Screen Resolution", "Remap controlls", "Toggle sound", "Back" };
		selectedOption = 0;

		const auto title = makeEntity();
		auto titleText = title->addComponent<TextComponent>("Options");

		for (int i = 0; i < numOfOptions; i++)
		{
			options.push_back(makeEntity());
			texts.push_back(options[i]->addComponent<TextComponent>(optionTexts[i]));
			if (i != 0)
			{
				texts[i]->ChangeColor(grey);
			}
		}
		options[0]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.1f));
		options[1]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.2f));
		options[2]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.32f));
		options[3]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.47f));
	}
	setLoaded(true);
}

void Options::Update(const double& dt)
{
	this_thread::sleep_for(chrono::milliseconds(100));
	if (Keyboard::isKeyPressed(CONTROLS[0]))
	{
		if (selectedOption - 1 >= 0)
		{
			texts[selectedOption]->ChangeColor(grey);
			selectedOption--;
			texts[selectedOption]->ChangeColor(Color::White);
		}
	}
	if (Keyboard::isKeyPressed(CONTROLS[1]))
	{
		if (selectedOption + 1 < options.size())
		{
			texts[selectedOption]->ChangeColor(grey);
			selectedOption++;
			texts[selectedOption]->ChangeColor(Color::White);
		}
	}
	if (Keyboard::isKeyPressed(CONTROLS[4]))
	{
		options.clear();
		texts.clear();
		switch (selectedOption)
		{
		case 0: // Screen resolution
			break;
		case 1: // Remap controlls
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