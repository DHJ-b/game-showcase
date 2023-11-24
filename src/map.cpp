#include "map.h"

Map::Map() {}
Map::Map(Map &&other) { data = other.data; }

Map::Map(const Map &other) { data = other.data; }

sf::Vector2f Map::size() { return sf::Vector2f(data[0].size(), data.size()); }

bool Map::walkable(sf::Vector2i pos, bool asEnemy) {
  int tmp = pos.x;
  pos.x = pos.y;
  pos.y = tmp;
  if (pos.x < 0 || pos.x > data.size() || pos.y < 0 || pos.y > data[0].size())
    return false;
  return !(
      data[pos.x][pos.y] == '-' || (!asEnemy && data[pos.x][pos.y] == 's') ||
      (asEnemy && (data[pos.x][pos.y] == 'p' || data[pos.x][pos.y] == 'P')));
}

int Map::spawnPoints() {
  if (_spawnPoints == -1) {
    _spawnPoints = 0;
    for (auto &s : data)
      for (auto c : s)
        if (c == 'S')
          _spawnPoints++;
  }
  return _spawnPoints;
}

sf::Vector2i Map::spawnPoint(int index) {
  int cnt = 0;
  for (int y = 0; y < data.size(); y++)
    for (int x = 0; x < data[y].size(); x++) {
      char c = data[y][x];
      if (c == 'S') {
        if (cnt == index)
          return sf::Vector2i(x, y);
        cnt++;
      }
    }
  return sf::Vector2i(-1, -1);
}

int Map::enemySpawns() {
  if (!_enemySpawns.size()) {
    for (int y = 0; y < data.size(); y++)
      for (int x = 0; x < data[y].size(); x++)
        if (data[y][x] == 's')
          _enemySpawns.push_back({x, y});
    std::shuffle(_enemySpawns.begin(), _enemySpawns.end(),
                 std::default_random_engine{});
  }
  return _enemySpawns.size();
}

sf::Vector2i Map::enemySpawn(int index) {
  if (!_enemySpawns.size())
    enemySpawns();
  return sf::Vector2i(_enemySpawns[index].first, _enemySpawns[index].second);
}
bool Map::isFood(sf::Vector2i pos) {
  std::swap(pos.x, pos.y);
  if (pos.x < 0 || pos.x > data.size() || pos.y < 0 || pos.y > data[0].size())
    return false;
  char c = data[pos.x][pos.y];
  return c == '.' || c == '%' || c == '&';
}
sf::Vector2i Map::correspondingPortal(char p) {
  if ('a' <= p && p <= 'z')
    p += 'A' - 'a';
  else if ('A' <= p && p <= 'Z')
    p += 'a' - 'A';
  for (int y = 0; y < data.size(); y++)
    for (int x = 0; x < data[y].size(); x++)
      if (data[y][x] == p)
        return sf::Vector2i(x, y);
  return sf::Vector2i(-1, -1);
}
