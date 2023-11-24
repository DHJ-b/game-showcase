#pragma once
#include "ResourceManager.hpp"
#include <functional>
enum class Event {
  ENERGIZED,
  GOT_FRUIT,
  GOT_FOOD,
  DEAD,
  GAME_OVER,
  CHANGED_DIRECTION
  // todo
};
enum class Enemy { WIFE, BOSS, SON, DAUGHTER, NONE };
class Game {
private:
  Map map;
  Logger *const logger;
  std::pair<sf::Vector2f, sf::Vector2f> player, wife, boss, son, daughter;
  sf::Vector2f mapSize;
  float playerSpeed = 16, enemySpeed = 15;
  float difficulty = 1;
  float delta;
  std::function<void(Event)> callback = [](Event e) { return; };

  int points = 0;
  int health = 5;
  Enemy eater = Enemy::NONE;

  float energizedTimer = 0;
  const float ENERGIZE_SPEED_BONUS = 1.5f;

  const float RESERVE_ROTATION_TIME = 0.3f;
  float reservedTimer;
  sf::Vector2f reserved;

  bool justTeleported = false;
  float justTeleportedTimer = 0;

  // vim order

public:
  const float SCALE = 8;
  Game(Logger *const logger, std::shared_ptr<Map> map);
  void setCallback(std::function<void(Event)> callback);
  void update(float delta);
  void triggerRight();
  void triggerLeft();
  void triggerUp();
  void triggerDown();
  const Map &getMap();
  sf::Vector2f getPlayerPosition();
  sf::Vector2f getPlayerDirection();
  sf::Vector2f getWifePosition();
  sf::Vector2f getWifeDirection();
  sf::Vector2f getBossPosition();
  sf::Vector2f getBossDirection();
  sf::Vector2f getSonPosition();
  sf::Vector2f getSonDirection();
  sf::Vector2f getDaughterPosition();
  sf::Vector2f getDaughterDirection();
  int getScore();
  int getHealth();

private:
  sf::Vector2f calculateNewPosition(sf::Vector2f pos, sf::Vector2f vel,
                                    float speed, bool asEnemy = false);
  void processCollision(Enemy enemy,
                        std::pair<sf::Vector2f, sf::Vector2f> &enemyPoint);
  void processPlayer();
  void processWife();
  void processSon();
  void processDaughter();
  void processBoss();
  bool canGo(sf::Vector2i direction);
};
