#pragma once
#include "SFML/Graphics.hpp"
#include "logger.h"
#include "resourcemanager.h"
#include <SFML/System/Clock.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class Controller;

enum class State {
  MAIN_MENU = 0,
  RECORD_TABLE,
  LEVEL_SELECTOR,
  GAME,
  SETTINGS
};

class IController {
protected:
  Logger *const logger;
  sf::RenderWindow &window;
  std::unordered_map<std::string, std::string> config;
  State &root;
  ResourceManager &manager;

public:
  IController(State &root, Logger *const logger, sf::RenderWindow &window,
              std::unordered_map<std::string, std::string> &config,
              ResourceManager &manager);
  virtual void update(float delta) = 0;
  virtual void pressUp();
  virtual void pressLeft();
  virtual void pressRight();
  virtual void pressDown();
  virtual void pressEsc();
  virtual void mouseDown();
  virtual void mouseUp();
  virtual void pressEnter();
  virtual void mouseMove(float x, float y);
};

class Controller {
private:
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
  Controller(Logger *const logger, const std::string configPath);
  ~Controller();
  void run();

private:
  void loadConfig(std::string const &path);
  void handleInput();
};

class MainMenu : public IController {
private:
public:
  MainMenu(State &root, Logger *const logger, sf::RenderWindow &window,
           std::unordered_map<std::string, std::string> &config,
           ResourceManager &manager);
  void update(float delta) override;
};
