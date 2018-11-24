#ifndef LOOPCLOCK_H
#define LOOPCLOCK_H

#include <Arduino.h>

namespace loopClock {
  using Time = decltype(::micros());
  using Millis = decltype(::millis());

  Time now();
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
