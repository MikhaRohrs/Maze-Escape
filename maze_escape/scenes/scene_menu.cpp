#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include "SFML/Window/Event.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void MenuScene::Load()
{
	cout << "Menu Load \n";
	{
		constexpr int numOfOptions = 4;
		const string optionTexts[numOfOptions] = { "Start", "Leaderboard", "Options", "Exit" };
		SelectedOption = 0;

		const auto title = makeEntity();
  		auto titleText = title->addComponent<TextComponent>("Maze Escape");

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

void MenuScene::Update(const double& dt)
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
		if(SelectedOption + 1 < MenuOptions.size())
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
		case 0: // Level select
			Engine::ChangeScene(&levelSelect);
			break;
		case 1: // Leaderboard
			Engine::ChangeScene(&leaderBoard);
			break;
		case 2: // MenuOptions
			Engine::ChangeScene(&optionScene);
			break;
		case 3: // Exit
			Engine::GetWindow().close();
			break;
		default:
			break;
		}
	}
	CoolDown -= dt;
	Scene::Update(dt);
}