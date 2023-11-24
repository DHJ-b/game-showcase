#pragma once
#include "SFML/Graphics.hpp"
#include <algorithm>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
struct Map {
  int _spawnPoints = -1;
  std::vector<std::string> data;
  std::vector<std::pair<int, int>> _enemySpawns;
  Map();
  Map(Map &&other);
  Map(const Map &other);
  sf::Vector2f size();
  bool walkable(sf::Vector2i pos, bool asEnemy = false);
  int spawnPoints();
  sf::Vector2i spawnPoint(int index);
  int enemySpawns();
  sf::Vector2i enemySpawn(int index);
  sf::Vector2i correspondingPortal(char p);
  bool isFood(sf::Vector2i pos);
};
