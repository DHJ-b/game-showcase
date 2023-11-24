#pragma once
#include "IController.hpp"

class Application {
private:
  enum class State {
    MAIN_MENU = 0,
    RECORD_TABLE,
    LEVEL_SELECTOR,
    GAME,
    SETTINGS
  };
  std::unordered_map<std::string, std::string> config;
  Logger *const logger;
  std::string const path;
  State state = State::MAIN_MENU;
  std::unique_ptr<IController> controllers[5];
  std::unique_ptr<sf::RenderWindow> window;
  sf::Clock clock;
  ResourceManager manager;
  sf::Keyboard::Scancode up, down, left, right;

public:
  Application(Logger *const logger, const std::string configPath);
  ~Application();
  void run();

private:
  void loadConfig(std::string const &path);
  void handleInput();
};
