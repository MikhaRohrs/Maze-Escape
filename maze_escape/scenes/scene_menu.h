#pragma once

#include "engine.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"

#define NUM_OF_OPTIONS 3

class MenuScene : public Scene
{
public:
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;

private:
    std::shared_ptr<Entity> options[NUM_OF_OPTIONS];
    std::shared_ptr<ShapeComponent> optionBox[NUM_OF_OPTIONS];
    int selectedOption = 0;
    sf::Color grey = sf::Color(50, 50, 50, 255);
};
