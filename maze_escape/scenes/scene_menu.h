#pragma once

#include "engine.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include <vector>

class MenuScene : public Scene
{
public:
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;

  std::vector<std::shared_ptr<Entity>> options;
  std::vector<std::shared_ptr<TextComponent>> texts;
  int selectedOption;
  sf::Color grey = sf::Color(50, 50, 50, 255);

private:
    
    
};
