#include "MenuController.hpp"
MainMenu::MainMenu(Logger *const logger, sf::RenderWindow &window,
                   std::unordered_map<std::string, std::string> &config,
                   ResourceManager &manager)
    : IController(logger, window, config, manager) {
  logger->log("Main Screen created", 2);
}
void MainMenu::update(float delta) {}
