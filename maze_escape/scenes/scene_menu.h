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

protected:
  std::vector<std::shared_ptr<Entity>> MenuOptions;
  std::vector<std::shared_ptr<TextComponent>> Texts;
  int SelectedOption;
  sf::Color Grey = sf::Color(50, 50, 50, 255);
  float CoolDown = 0.2f;
};
