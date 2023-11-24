#include "Game.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

Game::Game(Logger *const logger, std::shared_ptr<Map> map)
    : logger(logger), map(*map) { // something
  logger->log("inside game constr", 1);
  mapSize = map->size();
  mapSize.x *= SCALE;
  mapSize.y *= SCALE;
  triggerRight();
  player.second.x = playerSpeed;
  for (auto &s : map->data) {
    for (auto c : s)
      std::cout << c;
    std::cout << std::endl;
  }
  std::srand(std::time(0));
  int point = std::rand() % (this->map.spawnPoints());
  auto pointPos = this->map.spawnPoint(point);
  logger->log("player spawned at x = " + std::to_string(pointPos.x) +
                  ", y = " + std::to_string(pointPos.y),
              1);
  player.first.x = pointPos.x * SCALE + 0.5 * SCALE;
  player.first.y = pointPos.y * SCALE + 0.5 * SCALE;
  logger->log("player's actual in-game position at x = " +
                  std::to_string(player.first.x) +
                  ", y = " + std::to_string(player.first.y),
              1);
  wife.first = (sf::Vector2f(map->enemySpawn(rand() % map->enemySpawns())) +
                sf::Vector2f(0.5f, 0.5f)) *
               SCALE;
}

sf::Vector2i convert(sf::Vector2f pos, float scale) {
  return sf::Vector2i(0.01f + pos.x / scale, 0.01f + pos.y / scale);
}
float len(sf::Vector2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }
sf::Vector2f normalized(sf::Vector2f v) {
  float sq = 1. / len(v);
  return sf::Vector2f(v.x * sq, v.y * sq);
}
void Game::update(float delta) {
  this->delta = delta * difficulty;
  energizedTimer -= this->delta;
  reservedTimer -= delta;
  justTeleportedTimer -= delta;
  if (justTeleportedTimer < 0) {
    justTeleportedTimer = 0;
    justTeleported = false;
  }
  if (energizedTimer < 0) {
    energizedTimer = 0;
    player.second = normalized(player.second) * playerSpeed;
  } else {
    player.second =
        normalized(player.second) * playerSpeed * ENERGIZE_SPEED_BONUS;
  }
  auto cpy = player.first;
  cpy /= SCALE;
  cpy.x = (std::floor(cpy.x) + 0.5f) * SCALE;
  cpy.y = (std::floor(cpy.y) + 0.5f) * SCALE;
  if (len(reserved) > 0.5f) {
    logger->log("inside reserved, x = " + std::to_string(reserved.x) +
                    ", y = " + std::to_string(reserved.y) +
                    "\nreserved_timer = " + std::to_string(reservedTimer),
                1);
    logger->log("p.x = " + std::to_string(player.first.x) +
                    ", p.y = " + std::to_string(player.first.y) +
                    "; cp.x = " + std::to_string((cpy).x) +
                    "; cp.y = " + std::to_string((cpy).y),
                1);
    if (reservedTimer > 0 && len(player.first - cpy) <= 0.1f) {
      player.second = reserved * playerSpeed;
      player.first = cpy;
      logger->log("changed player speed from reserved, x = " +
                      std::to_string(player.second.x) +
                      ", y = " + std::to_string(player.second.y),
                  1);
      reservedTimer = -0.1;
    }
  }
  if (reservedTimer < 0) {
    reserved.x = 0.f;
    reserved.y = 0.f;
    reservedTimer = 0;
  } // finally working, do not touch
  processPlayer();
  processWife();
  processSon();
  processDaughter();
  processBoss();

  // eatfood:
  auto coords = convert(player.first, SCALE);
  for (int dy = -1; dy < 2; dy++)
    for (int dx = -1; dx < 2; dx++) {
      auto potFood = sf::Vector2i(coords.x + dx, coords.y + dy);
      if (!map.isFood(potFood))
        continue;
      float distance = len(sf::Vector2f(potFood) * SCALE +
                           sf::Vector2f(0.5f, 0.5f) * SCALE - player.first);
      if (distance < 5.5f) {
        char foodType = map.data[potFood.y][potFood.x];
        if (foodType == '.') {
          points += 10;
          callback(Event::GOT_FOOD);
        }
        if (foodType == '&') {
          points += 50;
          energizedTimer = 8.f;
          player.second *= ENERGIZE_SPEED_BONUS;
          callback(Event::ENERGIZED);
        }
        if (foodType == '%') {
          points += 100;
          callback(Event::GOT_FOOD);
        }
        map.data[potFood.y][potFood.x] = ' ';
      }
    }
  // food is fine

  // portal:
  char potPor = map.data[coords.y][coords.x];
  if ((potPor == 'p' || potPor == 'P') &&
      len(sf::Vector2f(coords) * SCALE + sf::Vector2f(0.5f, 0.5f) * SCALE -
          player.first) < 0.1 &&
      !justTeleported) {
    auto npos = map.correspondingPortal(potPor);
    player.first.x = npos.x + 0.5f;
    player.first.y = npos.y + 0.5f;
    player.first *= SCALE;
    justTeleported = true;
    justTeleportedTimer = 0.1;
  }

  // collide with enemies:
  processCollision(Enemy::WIFE, wife);
  processCollision(Enemy::BOSS, boss);
  processCollision(Enemy::SON, son);
  processCollision(Enemy::DAUGHTER, daughter);
}

void Game::processCollision(Enemy enemy,
                            std::pair<sf::Vector2f, sf::Vector2f> &enemyPoint) {
  auto distance = len(player.first - enemyPoint.first);
  if (distance < 5.5f) {
    if (energizedTimer > 0) {
      // possiblyEatEnemy
    } else {
      health--;
      eater = enemy;
      if (health == 0) {
        callback(Event::GAME_OVER);
        return;
      }
      callback(Event::DEAD);
      auto spawn = map.spawnPoint(rand() % map.spawnPoints());
      player.first.x = spawn.x + 0.5f;
      player.first.y = spawn.y + 0.5f;
      player.first *= SCALE;
    }
  }
}

sf::Vector2f Game::calculateNewPosition(sf::Vector2f pos, sf::Vector2f vel,
                                        float speed, bool asEnemy) {
  auto dt = vel * delta;
  logger->log("delta in game " + std::to_string(delta), 1);
  logger->log("delta pos x = " + std::to_string(dt.x) +

                  ", y = " + std::to_string(dt.y),
              1);
  auto truePot = dt + pos;
  auto nspeed = normalized(vel);
  auto pot = convert(pos + dt + nspeed * 0.5f * SCALE, SCALE);
  //  + normalized(vel) * (SCALE * 0.5f)
  logger->log("potential pos: x = " + std::to_string(pot.x) +
                  ", y = " + std::to_string(pot.y) +
                  " walkable = " + std::to_string(map.walkable(pot, asEnemy)),
              1);
  // map.data[pot.y][pot.x] != '-'
  if (map.walkable(pot, asEnemy)) {
    return truePot;
  } else
    return pos;
}
void Game::processPlayer() {
  // 4 4 is pos
  auto [pos, vel] = player;
  pos = calculateNewPosition(pos, vel,
                             playerSpeed); // collision with map into account
  player.first = pos;
}
void Game::processWife() {
  auto pos = convert(wife.first, SCALE);
  sf::Vector2f centered(pos);
  centered.x += 0.5f;
  centered.y += 0.5f;
  centered *= SCALE;
  if (len(centered - wife.first) < 0.01) {
    wife.first = centered;
    pos.x += 1;
    std::vector<sf::Vector2f> dirs;
    if (map.walkable(pos, true)) {
      dirs.push_back(sf::Vector2f(1, 0));
      logger->log("row = " + std::to_string(pos.y) +
                      ", col = " + std::to_string(pos.x),
                  3);
    }
    pos.x -= 2;
    if (map.walkable(pos, true)) {

      dirs.push_back(sf::Vector2f(-1, 0));
      logger->log("row = " + std::to_string(pos.y) +
                      ", col = " + std::to_string(pos.x),
                  3);
    }
    pos.x += 1;
    pos.y += 1;
    if (map.walkable(pos, true)) {
      dirs.push_back(sf::Vector2f(0, 1));
      logger->log("row = " + std::to_string(pos.y) +
                      ", col = " + std::to_string(pos.x),
                  3);
    }
    pos.y -= 2;
    if (map.walkable(pos, true)) {
      dirs.push_back(sf::Vector2f(0, -1));
      logger->log("row = " + std::to_string(pos.y) +
                      ", col = " + std::to_string(pos.x),
                  3);
    }
    if (dirs.size() == 0)
      logger->log("POSSIBLE DIRECTIONS ARE EMPTY!", 5);
    if (len(wife.second) < 0.01) {
      wife.second = dirs[rand() % dirs.size()];
      wife.second *= enemySpeed;
      return;
    }
    auto norm = normalized(wife.second);
    bool currentAvailable = false;
    for (sf::Vector2f i : dirs)
      if (len(i - norm) < 0.1)
        currentAvailable = true;
    if (currentAvailable && (rand() % 10) >= 6) {
      wife.second = norm;
    } else {
      wife.second = dirs[rand() % dirs.size()];
    }
    wife.second *= enemySpeed;
  }
  logger->log("wife's vel: x = " + std::to_string(wife.second.x) +
                  ", y = " + std::to_string(wife.second.y),
              2);
  wife.first = calculateNewPosition(wife.first, wife.second, enemySpeed, true);
  // wife.first += wife.second * delta;
}
void Game::processBoss() {}
void Game::processSon() {}
void Game::processDaughter() {}
void Game::triggerUp() {
  if (canGo(sf::Vector2i(0, -1))) {
    player.second = sf::Vector2f(0, -1.f) * playerSpeed;
    return;
  }
  reservedTimer = RESERVE_ROTATION_TIME;
  reserved.x = 0;
  reserved.y = -1.f;
}
void Game::triggerDown() {
  if (canGo(sf::Vector2i(0, 1.f))) {
    player.second = sf::Vector2f(0, 1.f) * playerSpeed;
    return;
  }
  reservedTimer = RESERVE_ROTATION_TIME;
  reserved.x = 0;
  reserved.y = 1.f;
}
void Game::triggerLeft() {
  if (canGo(sf::Vector2i(-1, 0))) {
    player.second = sf::Vector2f(-1.f, 0) * playerSpeed;
    return;
  }
  reservedTimer = RESERVE_ROTATION_TIME;
  reserved.x = -1.f;
  reserved.y = 0.f;
}
void Game::triggerRight() {
  if (canGo(sf::Vector2i(1, 0))) {
    player.second = sf::Vector2f(1.f, 0) * playerSpeed;
    return;
  }
  //   return;
  reservedTimer = RESERVE_ROTATION_TIME;
  reserved.x = 1.f;
  reserved.y = 0.f;
}
bool Game::canGo(sf::Vector2i direction) {
  direction.x = abs(direction.x);
  direction.y = abs(direction.y);
  sf::Vector2f dir(direction);
  return len(dir - normalized(sf::Vector2f(fabs(player.second.x),
                                           fabs(player.second.y)))) < 0.1;
  // auto pos = player.first;
  // auto posi = convert(pos, SCALE); // + normalized(player.second) * 0.5f *
  // SCALE posi += direction; return map.walkable(posi);
}
const Map &Game::getMap() { return map; }
sf::Vector2f Game::getPlayerPosition() { return player.first; }

sf::Vector2f Game::getDaughterDirection() {
  return normalized(daughter.second);
}

sf::Vector2f Game::getDaughterPosition() { return daughter.first; }

sf::Vector2f Game::getSonDirection() { return normalized(son.second); }

sf::Vector2f Game::getSonPosition() { return son.first; }

sf::Vector2f Game::getBossDirection() { return normalized(boss.second); }

sf::Vector2f Game::getBossPosition() { return boss.first; }

sf::Vector2f Game::getWifeDirection() { return normalized(wife.second); }

sf::Vector2f Game::getWifePosition() { return wife.first; }

sf::Vector2f Game::getPlayerDirection() { return normalized(player.second); }
void Game::setCallback(std::function<void(Event)> callback) {
  this->callback = callback;
}
int Game::getScore() { return points; }
int Game::getHealth() { return health; }
