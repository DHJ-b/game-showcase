#include "IController.hpp"
IController::IController(Logger *const logger, sf::RenderWindow &window,
                         std::unordered_map<std::string, std::string> &config,
                         ResourceManager &manager)
    : logger(logger), window(window), config(config), manager(manager) {}
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
void IController::pressEnter() {
  logger->log("unimplemented Enter handler triggered", 2);
}
