#include "logger.h"

TwoLevelLogger::TwoLevelLogger(std::ostream &normal, std::ostream &error,
                               int minLevel, int errorLevel)
    : normal(normal), error(error), minLevel(minLevel), errorLevel(errorLevel) {
}
void TwoLevelLogger::log(std::string message, int level) {
  if (level >= errorLevel)
    error << "\x1B[31m" << message << "\033[0m\t\t" << std::endl;
  else if (level >= minLevel)
    normal << message << std::endl;
}
