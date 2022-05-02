#include "scene_levelSelect.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "SFML/Window/Event.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void LevelSelect::Load()
{
	cout << "Level select load\n";
	{
		constexpr int numOfOptions = 4;
		const string optionTexts[numOfOptions] = { "Level 1", "Level 2", "Level 3", "Back" };
		SelectedOption = 0;

		const auto title = makeEntity();
		auto titleText = title->addComponent<TextComponent>("Maze Escape/Level Select");

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
			MenuOptions[i]->setPosition(Vector2f(70.0f, gameHeight - (gameHeight + offset)));
		}
	}
	setLoaded(true);
}

void LevelSelect::Update(const double& dt)
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
		case 0: // Level 1
			Engine::ChangeScene(&level1);
			break;
		case 1: // Level 2
			Engine::ChangeScene(&level2);
			break;
		case 2: // Level 3
			Engine::ChangeScene(&level3);
			break;
		case 3: // Menu
			Engine::ChangeScene(&menu);
			break;
		default:
			break;
		}
	}
	MenuScene::Update(dt);
}
