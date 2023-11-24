#include "gamecontroller.h"
#include <cmath>
GameController::GameController(
    State &root, Logger *const logger, sf::RenderWindow &window,
    std::unordered_map<std::string, std::string> &config,
    ResourceManager &manager)
    : IController(root, logger, window, config, manager), SCALE(8),
      game(std::make_unique<Game>(logger, manager.requireMap("map1")))

{
  initCallback();
  game->setCallback(this->_callback);
  logger->log("before loading textures", 1);
  playerTexture = manager.requireTexture("player");
  sonTexture = manager.requireTexture("son");
  wifeTexture = manager.requireTexture("wife");
  daughterTexture = manager.requireTexture("daughter");
  bossTexture = manager.requireTexture("boss");
  fruitTexture = manager.requireTexture("%");
  energizerTexture = manager.requireTexture("&");
  foodTexture = manager.requireTexture(".");
  logger->log("textures loaded", 1);
  logger->log("loading font: ", 2);
  uiFont = manager.requireFont("karmatic-font");

  player.setTexture(*playerTexture);
  sf::Vector2i ts(playerTexture->getSize());
  ts /= 2;
  player.setOrigin(ts.x, ts.y);
  wife.setTexture(*wifeTexture);
  wife.setOrigin(ts.x, ts.y);
  son.setTexture(*sonTexture);
  son.setOrigin(ts.x, ts.y);
  daughter.setTexture(*daughterTexture);
  daughter.setOrigin(ts.x, ts.y);
  boss.setTexture(*bossTexture);
  boss.setOrigin(ts.x, ts.y);
  food.setTexture(*foodTexture);
  fruit.setTexture(*fruitTexture);
  energizer.setTexture(*energizerTexture);
  wall = sf::RectangleShape(sf::Vector2f(64, 64));
  wall.setOutlineThickness(-2.f);
  wall.setOutlineColor(sf::Color(56, 182, 0));
  wall.setFillColor(sf::Color::Transparent);
  hps.setTexture(*playerTexture);
  hps.setScale(0.5f, 0.5f);
  hps.setOrigin(16, 16);
  hps.setRotation(45.f);
}
void GameController::update(float delta) {
  // process input here
  this->delta = delta;
  if (state == PrivateState::PLAYING)
    game->update(delta);
  if (state == PrivateState::PLAYING || state == PrivateState::PAUSE ||
      state == PrivateState::DEAD_SCREEN)
    drawGameScreen();
  if (state == PrivateState::PAUSE) {
    processPause();
    drawPause();
  }
  if (state == PrivateState::DEAD_SCREEN) {
    processDead();
    drawDead();
  }
  if (state == PrivateState::GAME_OVER_SCREEN) {
    processGameOver();
  }
  if (state == PrivateState::GAME_OVER_SCREEN ||
      state == PrivateState::REPLAY_MENU) {
    drawGameOver();
    processMenu();
    drawMenu();
  }
  window.display();
}

void GameController::pressUp() {
  if (state == PrivateState::PLAYING)
    game->triggerUp();
  if (state == PrivateState::REPLAY_MENU)
    selected = std::min(selected + 1, MAX_SELECTED);
}

void GameController::pressDown() {
  if (state == PrivateState::PLAYING)
    game->triggerDown();
  if (state == PrivateState::REPLAY_MENU)
    selected = std::max(selected - 1, 0);
}

void GameController::pressLeft() {
  if (state == PrivateState::PLAYING)
    game->triggerLeft();
}

void GameController::pressRight() {
  if (state == PrivateState::PLAYING)
    game->triggerRight();
}

void GameController::pressEsc() {
  if (state == PrivateState::PAUSE)
    state = PrivateState::PLAYING;
  else if (state == PrivateState::PLAYING)
    state = PrivateState::PAUSE;
}
void callback(Event event) {}
void GameController::initCallback() {
  this->_callback = [this](Event event) -> void {
    switch (event) {
    case Event::ENERGIZED:
      break;
    case Event::DEAD:
      state = PrivateState::DEAD_SCREEN;
      deadTimer = DEAD_TIMER_INIT;
      break;
    case Event::GOT_FOOD:
      break;
    case Event::GAME_OVER:
      state = PrivateState::GAME_OVER_SCREEN;
      gameOverTimer = GAME_OVER_TIMER_INIT;
      break;
    case Event::GOT_FRUIT:
      break;
    case Event::CHANGED_DIRECTION:
      break;
    };
  };
}
void GameController::drawGameScreen() {
  window.clear(sf::Color::Black);
  auto &mp = game->getMap();
  topLeft.x = 100;
  for (int i = 0; i < mp.data.size(); i++) {
    for (int j = 0; j < mp.data[i].size(); j++) {
      sf::Vector2f position(j * (float)SCALE * game->SCALE,
                            i * (float)SCALE * game->SCALE);
      position += topLeft;
      switch (mp.data[i][j]) {
      case '.':
        food.setPosition(position);
        window.draw(food);
        break;
      case '&':
        energizer.setPosition(position);
        window.draw(energizer);
        break;
      case '-':
        wall.setPosition(position);
        window.draw(wall);
        break;
      case '%':
        fruit.setPosition(position);
        window.draw(fruit);
        break;
      }
    }
  }
  auto pd = game->getPlayerDirection();
  auto pp = game->getPlayerPosition();
  pp *= (float)SCALE;
  player.setPosition(topLeft + pp);
  auto wp = game->getWifePosition();
  wife.setPosition(topLeft + wp * SCALE);
  auto dp = game->getDaughterPosition();
  daughter.setPosition(topLeft + dp * SCALE);
  auto sp = game->getSonPosition();
  son.setPosition(topLeft + dp * SCALE);
  auto bp = game->getBossPosition();
  boss.setPosition(topLeft + bp * SCALE);
  window.draw(wife);
  // window.draw(son);
  // window.draw(daughter);
  // window.draw(boss);
  window.draw(player);
  sf::Text score;
  score.setFont(*uiFont);
  int sc = game->getScore();
  score.setString(std::to_string(sc));
  score.setPosition(topLeft - sf::Vector2f(0, 100.f));
  window.draw(score);
  int hp = game->getHealth();
  sf::Vector2f bl(topLeft.x, topLeft.y + 600);
  for (int i = 0; i < hp; i++) {
    hps.setPosition(bl + sf::Vector2f(32.f * i, 0));
    window.draw(hps);
  }
}
void GameController::drawPause() {}

void GameController::processPause() {}

void GameController::drawDead() {
  sf::Text text;
  text.setFont(*uiFont);
  text.setString("OUCH!");
  text.setCharacterSize(30);
  text.setFillColor(
      sf::Color(235, 64, 52, 255.f * (deadTimer / DEAD_TIMER_INIT)));
  text.setPosition(300, 300);
  window.draw(text);
}

void GameController::processDead() {
  deadTimer -= delta;
  if (deadTimer <= 0) {
    deadTimer = 0;
    state = PrivateState::PLAYING;
  }
}

void GameController::drawGameOver() {
  sf::Text text;
  text.setFont(*uiFont);
  text.setString("game->OvEr.");
  text.setCharacterSize(40);
  text.setFillColor(
      sf::Color((255, 255.f * (gameOverTimer / GAME_OVER_TIMER_INIT),
                 255.f * (gameOverTimer / GAME_OVER_TIMER_INIT))));
  text.setPosition(400, 200);
  window.draw(text);
}

void GameController::processGameOver() {
  gameOverTimer -= delta;
  bool showMenu = gameOverTimer <= 0;
  if (showMenu) {
    gameOverTimer = 0;
    state = PrivateState::REPLAY_MENU;
  }
}
void GameController::processMenu() {}

void GameController::drawMenu() {
  sf::Text replay, quit;
  replay.setFont(*uiFont);
  quit.setFont(*uiFont);
  replay.setCharacterSize(30);
  quit.setCharacterSize(30);
  replay.setString("replay");
  quit.setString("quit");
  replay.setPosition(400, 300);
  quit.setPosition(400, 350);
  auto clr = sf::Color(200, 160, 160);
  if (selected == 1) {
    replay.setFillColor(clr);
  } else if (selected == 0) {
    quit.setFillColor(clr);
  }
  window.draw(replay);
  window.draw(quit);
}
void GameController::pressEnter() {
  if (state == PrivateState::REPLAY_MENU) {
    if (selected == 0) {
      this->game = std::make_unique<Game>(logger, manager.requireMap("map1"));
      this->game->setCallback(_callback);
      state = PrivateState::PLAYING;
    } else if (selected == 1) {
      window.close();
    }
  }
}
