#pragma once
#include "gamecontroller.h"

struct Callback {
  GameController *controller;
  Callback(GameController *controller);
  void operator()(Event e);
};
