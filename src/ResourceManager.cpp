#include "ResourceManager.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <fstream>
ResourceManager::ResourceManager(
    std::unordered_map<std::string, std::string> &config, Logger *const logger)
    : logger(logger), config(config) {
  sf::RenderTexture rt;
  auto circ = sf::CircleShape(20);
  circ.setFillColor(sf::Color::Green);
  rt.draw(circ);
  textures["EMPTY"] = std::make_shared<sf::Texture>();
  *textures["EMPTY"] = rt.getTexture();
}

std::shared_ptr<const sf::Texture>
ResourceManager::requireTexture(std::string label) {
  if (textures.find(label) != textures.end())
    return textures[label];
  if (config.find(label) == config.end()) {
    config[label] = "PLACEHOLDER #required";
    logger->log("required " + label + " but is not found in config", 3);
    return textures["EMPTY"];
  }
  std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
  if (!texture->loadFromFile(config[label])) {
    logger->log("required " + label + " but can not open file " + config[label],
                3);
    return textures["EMPTY"];
  }
  textures[label] = texture;
  return texture;
}

Map parseMap(std::istream &in, Logger *const logger) {
  logger->log("before parsing map", 1);
  std::string str;
  std::getline(in, str);
  Map res;
  while (!in.eof()) {
    str = str.substr(0, str.find_first_of("#"));
    if (str.size() == 0) {
      std::getline(in, str);
      continue;
    }
    logger->log(str, 3);
    res.data.push_back(str);
    std::getline(in, str);
  }
  return res;
}

std::shared_ptr<const sf::SoundBuffer>
ResourceManager::requireSound(std::string label) {
  return nullptr;
}
std::shared_ptr<const sf::Font>
ResourceManager::requireFont(std::string label) {
  if (fonts.find(label) != fonts.end())
    return fonts[label];
  if (config.find(label) == config.end()) {
    logger->log("required " + label + " but is not found in config", 3);
    return fonts["EMPTY"];
  }
  fonts[label] = std::make_shared<sf::Font>();
  fonts[label]->loadFromFile(config[label]);
  return fonts[label];
}
std::shared_ptr<Map> ResourceManager::requireMap(std::string label) {
  if (maps.find(label) != maps.end())
    return maps[label];
  logger->log("loading map with label " + label, 1);
  if (config.find(label) == config.end()) {
    config[label] = "PLACEHOLDER #required";
    logger->log("required " + label + " but is not found in config", 3);
    return maps["EMPTY"];
  }
  std::ifstream fin;
  try {
    fin.open(config[label]);
  } catch (std::exception e) {
    logger->log("required " + label + " but can not open file " + config[label],
                3);
    fin.close();
    return maps["EMPTY"];
  }
  auto map = parseMap(fin, logger);
  fin.close();
  maps[label] = std::make_shared<Map>(map);
  logger->log("map has been parsed", 1);
  return maps[label];
}
