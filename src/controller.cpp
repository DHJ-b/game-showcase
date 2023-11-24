#include "controller.h"
#include "SFML/Graphics.hpp"
#include "gamecontroller.h"
#include <cstring>
#include <exception>
#include <fstream>
#include <string>
Controller::Controller(Logger *const logger, const std::string configPath)
    : logger(logger), path(configPath),
      manager(ResourceManager(config, logger)) {
  logger->log("config path: " + configPath, 2);
  loadConfig(configPath);
  logger->log("config loaded", 2);
  window = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(std::atoi(config["width"].c_str()),
                    std::atoi(config["height"].c_str())),
      config["title"]);
  state = State::GAME;
  config["upKey"];
  up = (sf::Keyboard::Scancode)(std::atoi(config["upKey"].c_str()));
  right = (sf::Keyboard::Scancode)(std::atoi(config["rightKey"].c_str()));
  left = (sf::Keyboard::Scancode)(std::atoi(config["leftKey"].c_str()));
  down = (sf::Keyboard::Scancode)(std::atoi(config["downKey"].c_str()));
  controllers[(int)State::MAIN_MENU] =
      std::make_unique<MainMenu>(state, logger, *window, config, manager);
  controllers[(int)State::GAME] =
      std::make_unique<GameController>(state, logger, *window, config, manager);
  // controllers[]
  clock = sf::Clock();
}

Controller::~Controller() {}

void Controller::loadConfig(std::string const &path) {
  std::ifstream fin;
  try {
    fin.open(path);
  } catch (std::exception *e) {
    logger->log(e->what(), 5);
    fin.close();
    throw std::exception();
  }
  logger->log("file opened: " + path, 1);
  std::string key, value;
  std::string str;
  while (getline(fin, str)) {
    str = str.substr(0, str.find_first_of("#"));
    if (str.size() == 0)
      continue;
    size_t space = str.find(' ');
    key = str.substr(0, space);
    value = str.substr(space + 1);
    if (key.size() == 0 || value.size() == 0)
      continue;
    config[key] = value;
    logger->log(key + "->" + value, 1);
  }
  fin.close();
}

void Controller::run() {
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  while (window->isOpen()) {
    sf::Time time = clock.restart();
    float delta = time.asMicroseconds() * 0.000001;
    logger->log("dt: " + std::to_string(delta), 1);
    handleInput();
    controllers[(int)state]->update(delta);
  }
}

void Controller::handleInput() {
  sf::Event event;
  while (window->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window->close();
      continue;
    }
    if (event.type == sf::Event::KeyPressed) {
      auto &cur = *controllers[(int)state];
      if (event.key.code == sf::Keyboard::localize(up))
        cur.pressUp();
      else if (event.key.code == sf::Keyboard::localize(down))
        cur.pressDown();
      else if (event.key.code == sf::Keyboard::localize(left))
        cur.pressLeft();
      else if (event.key.code == sf::Keyboard::localize(right))
        cur.pressRight();
      else if (event.key.code == sf::Keyboard::Escape)
        cur.pressEsc();
      else if (event.key.code == sf::Keyboard::Enter)
        cur.pressEnter();
    }
  }
}

IController::IController(State &root, Logger *const logger,
                         sf::RenderWindow &window,
                         std::unordered_map<std::string, std::string> &config,
                         ResourceManager &manager)
    : root(root), logger(logger), window(window), config(config),
      manager(manager) {}
void IController::pressUp() {
  logger->log("unimplemented Up handler triggered", 2);
}
void IController::pressDown() {
  logger->log("unimplemented Down handler triggered", 2);
}
void IController::pressLeft() {
  logger->log("unimplemented Left handler triggered", 2);
}
void IController::pressRight() {
  logger->log("unimplemented Right handler triggered", 2);
}
void IController::pressEsc() {
  logger->log("unimplemented Escape handler triggered", 2);
}
void IController::mouseUp() {
  logger->log("unimplemented MouseUp handler triggered", 2);
}
void IController::mouseDown() {
  logger->log("unimplemented MouseDown handler triggered", 2);
}
void IController::mouseMove(float x, float y) {
  logger->log("unimplemented Moved handler triggered", 2);
}
MainMenu::MainMenu(State &root, Logger *const logger, sf::RenderWindow &window,
                   std::unordered_map<std::string, std::string> &config,
                   ResourceManager &manager)
    : IController(root, logger, window, config, manager) {
  logger->log("Main Screen created", 2);
}
void MainMenu::update(float delta) {}
void IController::pressEnter() {
  logger->log("unimplemented Enter handler triggered", 2);
}
