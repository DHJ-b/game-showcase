#pragma once
#include "Game.hpp"
#include "IController.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

class GameController : public IController {
private:
  enum class PrivateState {
    PLAYING,
    PAUSE,
    DEAD_SCREEN,
    GAME_OVER_SCREEN,
    REPLAY_MENU
  };
  const float SCALE;
  std::shared_ptr<const sf::Texture> playerTexture, bossTexture, wifeTexture,
      sonTexture, daughterTexture, foodTexture, energizerTexture, fruitTexture;
  sf::Sprite player, boss, wife, son, daughter, food, energizer, fruit, hps;
  sf::Texture playerT;
  sf::RectangleShape wall;
  PrivateState state = PrivateState::PLAYING;
  std::unique_ptr<Game> game;
  std::shared_ptr<const sf::Font> uiFont;
  sf::Vector2f topLeft = {300.f, 100.f};
  std::function<void(Event)> _callback;
  float delta;
  float deadTimer = 0;
  const float DEAD_TIMER_INIT = 1.6f;
  float gameOverTimer = 0;
  const float GAME_OVER_TIMER_INIT = 0.5f;
  int selected = 0;
  const int MAX_SELECTED = 2;

public:
  GameController(Logger *const logger, sf::RenderWindow &window,
                 std::unordered_map<std::string, std::string> &config,
                 ResourceManager &manager);
  void update(float delta) override;
  void pressUp() override;
  void pressDown() override;
  void pressLeft() override;
  void pressRight() override;
  void pressEsc() override;
  void pressEnter() override;
  void initCallback();
  void drawGameScreen();
  void processGameOver();
  void drawGameOver();
  void processDead();
  void drawDead();
  void processPause();
  void drawPause();
  void processMenu();
  void drawMenu();
};
