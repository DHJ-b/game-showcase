#pragma once
#include "Logger.hpp"
#include "Map.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include <memory>
#include <unordered_map>
class ResourceManager {
  template <typename T> using dict = std::unordered_map<std::string, T>;

private:
  dict<std::string> const &config;
  dict<std::shared_ptr<sf::Texture>> textures;
  dict<std::shared_ptr<sf::SoundBuffer>> sounds;
  dict<std::shared_ptr<sf::Font>> fonts;
  dict<std::shared_ptr<Map>> maps;
  Logger *const logger;

public:
  ResourceManager(std::unordered_map<std::string, std::string> const &config,
                  Logger *const logger);
  std::shared_ptr<const sf::Texture> requireTexture(std::string label);
  std::shared_ptr<const sf::SoundBuffer> requireSound(std::string label);
  std::shared_ptr<const sf::Font> requireFont(std::string label);
  std::shared_ptr<Map> requireMap(std::string label);
};
