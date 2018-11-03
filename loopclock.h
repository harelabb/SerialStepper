#ifndef LOOPCLOCK_H
#define LOOPCLOCK_H

#include <Arduino.h>

namespace LoopClock {
  using Time = decltype(::micros());
  void tick();
  Time now();
}

#endif
