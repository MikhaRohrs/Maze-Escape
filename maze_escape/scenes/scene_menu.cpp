#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "SFML/Graphics/RectangleShape.hpp"

using namespace std;
using namespace sf;

void MenuScene::Load()
{
  cout << "Menu Load \n";
  {
    const auto title = makeEntity();
  	auto titleText = title->addComponent<TextComponent>("Maze Escape");

    for (int i = 0; i < NUM_OF_OPTIONS; i++)
    {
        options[i] = makeEntity();

        optionBox[i] = options[i]->addComponent<ShapeComponent>();
        optionBox[i]->setShape<RectangleShape>(Vector2f(120.0f, 50.0f));
        optionBox[i]->getShape().setFillColor(grey);
        optionBox[i]->getShape().setOrigin(Vector2f(60.0f, 25.0f));
    }

    options[0]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.1f));
    options[1]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.2f));
    options[2]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.32f));

    auto startText = options[0]->addComponent<TextComponent>("Start");
    startText->setPosition(options[0]->getPosition());
    startText->setOrigin(options[0]->getPosition() / 2.0f);
    
    auto leaderBoardText = options[1]->addComponent<TextComponent>("Leaderboard");
    leaderBoardText->setPosition(options[1]->getPosition());
    leaderBoardText->setOrigin(options[1]->getPosition() / 2.0f);

    auto exitText = options[2]->addComponent<TextComponent>("Exit");
    exitText->setPosition(options[2]->getPosition());
    exitText->setOrigin(options[2]->getPosition() / 2.0f);
  }
  setLoaded(true);
}

void MenuScene::Update(const double& dt)
{
  if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
  {
	  if (selectedOption - 1 >= 0)
	  {
	  	optionBox[selectedOption]->getShape().setFillColor(grey);
	  	selectedOption--;
        optionBox[selectedOption]->getShape().setFillColor(Color::Green);
	  }
  }
  if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
  {
      if(selectedOption + 1 < NUM_OF_OPTIONS)
      {
          optionBox[selectedOption]->getShape().setFillColor(grey);
          selectedOption++;
          optionBox[selectedOption]->getShape().setFillColor(Color::Green);
      }
  }


  if (sf::Keyboard::isKeyPressed(Keyboard::Space)) 
  {
    switch (selectedOption)
    {
    case 0:
        Engine::ChangeScene(&level1);
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        break;
    }
  }

  Scene::Update(dt);
}
