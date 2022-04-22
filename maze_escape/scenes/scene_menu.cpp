#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Event.hpp"

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

        // Commented out as I can not figure out how to have text be centered to the boxes. If fixed uncomment
        /*optionBox[i] = options[i]->addComponent<ShapeComponent>();
        optionBox[i]->setShape<RectangleShape>(Vector2f(120.0f, 50.0f));
        optionBox[i]->getShape().setFillColor(grey);
        optionBox[i]->getShape().setOrigin(Vector2f(60.0f, 25.0f));*/
    }

    options[0]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.1f));
    options[1]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.2f));
    options[2]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.32f));
    options[3]->setPosition(Vector2f(70.0f, GAME_HEIGHT[CURRENT_RES] - GAME_HEIGHT[CURRENT_RES] / 1.47f));

    texts[0] = options[0]->addComponent<TextComponent>("Start");

    texts[1] = options[1]->addComponent<TextComponent>("Leaderboard");
    texts[1]->ChangeColor(grey);

    texts[2] = options[2]->addComponent<TextComponent>("Options");
    texts[2]->ChangeColor(grey);

    texts[3] = options[3]->addComponent<TextComponent>("Exit");
    texts[3]->ChangeColor(grey);
  }
  setLoaded(true);
}

void MenuScene::Update(const double& dt)
{
  if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
  {
	  if (selectedOption - 1 >= 0)
	  {
	  	texts[selectedOption]->ChangeColor(grey);
	  	selectedOption--;
        texts[selectedOption]->ChangeColor(Color::White);
        cout << "going up\n";
	  }
  }
  if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
  {
      if(selectedOption + 1 < NUM_OF_OPTIONS)
      {
          texts[selectedOption]->ChangeColor(grey);
          selectedOption++;
          texts[selectedOption]->ChangeColor(Color::White);
      }
  }

  if (Keyboard::isKeyPressed(Keyboard::Space)) 
  {
    switch (selectedOption)
    {
    case 0: // Start
        Engine::ChangeScene(&level1);
        break;
    case 1: // Leaderboard
        Engine::ChangeScene(&leaderBoard);
        break;
    case 2: // Options
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
