#include "scene_options_controls.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "SFML/Window/Event.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

string KeycodeToString(Keyboard::Key& key)
{
	string keyAsString;
	#define ITEM(key) case Keyboard:: ## key : keyAsString = #key; break
	switch (key)
	{
		ITEM(A);
		ITEM(B);
		ITEM(C);
		ITEM(D);
		ITEM(E);
		ITEM(F);
		ITEM(G);
		ITEM(H);
		ITEM(I);
		ITEM(J);
		ITEM(K);
		ITEM(L);
		ITEM(M);
		ITEM(N);
		ITEM(O);
		ITEM(P);
		ITEM(Q);
		ITEM(R);
		ITEM(S);
		ITEM(T);
		ITEM(U);
		ITEM(V);
		ITEM(W);
		ITEM(X);
		ITEM(Y);
		ITEM(Z);
		ITEM(Num0);
		ITEM(Num1);
		ITEM(Num2);
		ITEM(Num3);
		ITEM(Num4);
		ITEM(Num5);
		ITEM(Num6);
		ITEM(Num7);
		ITEM(Num8);
		ITEM(Num9);
		ITEM(Escape);
		ITEM(LControl);
		ITEM(LShift);
		ITEM(LAlt);
		ITEM(LSystem);
		ITEM(RControl);
		ITEM(RShift);
		ITEM(RAlt);
		ITEM(RSystem);
		ITEM(Menu);
		ITEM(LBracket);
		ITEM(RBracket);
		ITEM(Semicolon);
		ITEM(Comma);
		ITEM(Period);
		ITEM(Quote);
		ITEM(Slash);
		ITEM(Backslash);
		ITEM(Tilde);
		ITEM(Equal);
		ITEM(Hyphen);
		ITEM(Space);
		ITEM(Enter);
		ITEM(Backspace);
		ITEM(Tab);
		ITEM(PageUp);
		ITEM(PageDown);
		ITEM(End);
		ITEM(Home);
		ITEM(Insert);
		ITEM(Delete);
		ITEM(Add);
		ITEM(Subtract);
		ITEM(Multiply);
		ITEM(Divide);
		ITEM(Left);
		ITEM(Right);
		ITEM(Up);
		ITEM(Down);
		ITEM(Numpad0);
		ITEM(Numpad1);
		ITEM(Numpad2);
		ITEM(Numpad3);
		ITEM(Numpad4);
		ITEM(Numpad5);
		ITEM(Numpad6);
		ITEM(Numpad7);
		ITEM(Numpad8);
		ITEM(Numpad9);
		ITEM(F1);
		ITEM(F2);
		ITEM(F3);
		ITEM(F4);
		ITEM(F5);
		ITEM(F6);
		ITEM(F7);
		ITEM(F8);
		ITEM(F9);
		ITEM(F10);
		ITEM(F11);
		ITEM(F12);
		ITEM(F13);
		ITEM(F14);
		ITEM(F15);
		ITEM(Pause);
	default:
		keyAsString = "Unknown";
		break;
	}
	return keyAsString;
}

void ControlOptions::Load()
{
	cout << "Options load\n";
	{
		constexpr int numOfOptions = 3;
		const string optionTexts[numOfOptions] = { "Change Controls", "Reset to Default", "Back" };
		SelectedOption = 0;

		const auto title = makeEntity();
		auto titleText = title->addComponent<TextComponent>("Options/Remap Controls");


		auto currentControls = makeEntity();
		auto currentControlsText = currentControls->addComponent<TextComponent>
		("Move Up: " + KeycodeToString(CONTROLS[0]) +
			"\nMove Down: " + KeycodeToString(CONTROLS[1]) +
			"\nMove Left: " + KeycodeToString(CONTROLS[2]) +
			"\nMove Right: " + KeycodeToString(CONTROLS[3]) +
			"\nInteract / Select: " + KeycodeToString(CONTROLS[4]));
		currentControls->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.1f));
		
		for (int i = 0; i < numOfOptions; i++)
		{
			MenuOptions.push_back(makeEntity());
			Texts.push_back(MenuOptions[i]->addComponent<TextComponent>(optionTexts[i]));
			if (i != 0)
			{
				Texts[i]->ChangeColor(Grey);
			}
		}

		MenuOptions[0]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.2f));
		MenuOptions[1]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.32f));
		MenuOptions[2]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.47f));
	}
	setLoaded(true);
}

void ControlOptions::Update(const double& dt)
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
		switch (SelectedOption)
		{
		case 0: // Begin process of remapping
			break;
		case 1: // Reset controls to default
			CONTROLS[0] = Keyboard::W;
			CONTROLS[1] = Keyboard::S;
			CONTROLS[2] = Keyboard::A;
			CONTROLS[3] = Keyboard::D;
			CONTROLS[4] = Keyboard::Space;
			break;
		case 2: // Back to options
			MenuOptions.clear();
			Texts.clear();
			Engine::ChangeScene(&optionScene);
			break;
		default:
			break;
		}
	}
	MenuScene::Update(dt);
}