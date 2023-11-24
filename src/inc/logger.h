#pragma once
#include <iostream>

class Logger {
public:
  virtual void log(std::string message, int level) = 0;
};

class TwoLevelLogger : public Logger {
private:
  std::ostream &normal;
  std::ostream &error;
  int minLevel;
  int errorLevel;

public:
  TwoLevelLogger(std::ostream &normal, std::ostream &error, int minLevel = 0,
                 int errorLevel = 2);
  void log(std::string message, int level) override;
};
