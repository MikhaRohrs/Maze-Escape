#include "scene_leaderboard.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "SFML/Window/Event.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

string leaderBoardLocation = "res/Leaderboard.txt";

void LeaderBoard::ReadLeaderboardFile()
{
	ifstream file;
	string line;
	file.open(leaderBoardLocation);
	if(!file.is_open())
	{
		cout << "Leaderboard file not found\n";
		return;
	}

	while (getline(file, line))
	{
		if (line.empty())
		{
			return;
		}
		ScoreList.push_back(line);
	}

	sort(ScoreList.begin(), ScoreList.end());
	reverse(ScoreList.begin(), ScoreList.end());

	file.close();
}

void LeaderBoard::ResetLeaderboardFile()
{
	ofstream file{ leaderBoardLocation };
	file.close();
	ScoreList.clear();
	ReadLeaderboardFile();
}

void LeaderBoard::Load()
{
	cout << "Leaderboard load\n";
	{
		ReadLeaderboardFile();

		if (ScoreList.empty())
		{
			auto empty = makeEntity();
			auto emptyText = empty->addComponent<TextComponent>("No scores yet");
			empty->setPosition(Vector2f(70.0f, gameHeight - (gameHeight - 50.0f)));
		}
		else
		{
			string fullString;
			for (int i = 0; i < ScoreList.size(); i++)
			{
				fullString += to_string(i + 1) + ".: " + ScoreList[i] + "\n";
			}
			auto score = makeEntity();
			auto scoreText = score->addComponent<TextComponent>(fullString);
			score->setPosition(Vector2f(70.0f, gameHeight - (gameHeight - 50.0f)));
		}


		constexpr int numOfOptions = 2;
		const string optionTexts[numOfOptions] = { "Reset", "Back" };
		SelectedOption = 0;

		const auto title = makeEntity();
		auto titleText = title->addComponent<TextComponent>("Maze Escape/Leaderboard");

		float offset = -450.0f;
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

void LeaderBoard::Update(const double& dt)
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
		case 0: // Reset leaderboard
			ResetLeaderboardFile();
			Engine::ChangeScene(&leaderBoard);
			break;
		case 1: // Main menu
			Engine::ChangeScene(&menu);
			break;
		default:
			break;
		}
	}
	MenuScene::Update(dt);
}