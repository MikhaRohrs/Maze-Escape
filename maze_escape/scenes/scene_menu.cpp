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
		selectedOption = 0;

		const auto title = makeEntity();
  		auto titleText = title->addComponent<TextComponent>("Maze Escape");

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

void MenuScene::Update(const double& dt)
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
		if(selectedOption + 1 < options.size())
		{
      		texts[selectedOption]->ChangeColor(grey);
      		selectedOption++;
      		texts[selectedOption]->ChangeColor(Color::White);
		}
	}
	if (Keyboard::isKeyPressed(CONTROLS[4])) 
	{
		switch (selectedOption)
		{
		case 0: // Level select
			options.clear();
			texts.clear();
			Engine::ChangeScene(&levelSelect);
			break;
		case 1: // Leaderboard
			options.clear();
			texts.clear();
			Engine::ChangeScene(&leaderBoard);
			break;
		case 2: // Options
			options.clear();
			texts.clear();
			Engine::ChangeScene(&optionScene);
			break;
		case 3: // Exit
			Engine::GetWindow().close();
			break;
		default:
			break;
		}
	}
	Scene::Update(dt);
}