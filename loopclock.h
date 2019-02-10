#ifndef LOOPCLOCK_H
#define LOOPCLOCK_H

#include <Arduino.h>

namespace loopClock {
  using Micros = decltype(::micros());
  using Millis = decltype(::millis());

  Micros now();
  void tick();

  class Timer {
  public:
    bool set(float secs);
    bool wait();
  private:
    Millis time_ = 0;
  };
}

#endif
