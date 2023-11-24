#include "controller.h"
#include "logger.h"
#include "map.h"
#include <iostream>
#include <memory>

int main() {

  std::unique_ptr<Logger> logger =
      std::make_unique<TwoLevelLogger>(std::cout, std::cerr, 2, 3);
  Controller controller(&*logger, "resources/config.ini");
  controller.run();
}
