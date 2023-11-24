#include "Application.hpp"
#include <iostream>
#include <memory>

int main() {

  std::unique_ptr<Logger> logger =
      std::make_unique<TwoLevelLogger>(std::cout, std::cerr, 2, 3);
  Application game(&*logger, "resources/config.ini");
  game.run();
}
