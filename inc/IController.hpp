#pragma once
#include "Logger.hpp"
#include "ResourceManager.hpp"
#include "SFML/Graphics.hpp"
#include <functional>
#include <unordered_map>

class IController {
protected:
  Logger *const logger;
  sf::RenderWindow &window;
  std::unordered_map<std::string, std::string> config;
  ResourceManager &manager;
  // std::function<void(ApplicationEvent)> callback;

public:
  IController(Logger *const logger, sf::RenderWindow &window,
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
  // virtual void addCallback(std::function<void(ApplicationEvent)> callback);
};
