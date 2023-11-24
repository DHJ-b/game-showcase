#pragma once
#include "IController.hpp"
class MainMenu : public IController {
private:
public:
  MainMenu(Logger *const logger, sf::RenderWindow &window,
           std::unordered_map<std::string, std::string> &config,
           ResourceManager &manager);
  void update(float delta) override;
};
