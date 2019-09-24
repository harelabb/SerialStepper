#ifndef LOOPCLOCK_H
#define LOOPCLOCK_H

#include <Arduino.h>

/// @file

/// Utility to check the clock only once per loop.
namespace loopClock {

  /// Use the type that micros() returns to store microseconds.
  using Micros = decltype(::micros());

  /// Get microseconds from the last call to tick().
  Micros microsNow();

  /// Stores the return value from ::micros().
  /// The value can be retrieved by microsNow().
  /// \n Should be call once per loop.
  void tick();

  using Millis = decltype(::millis());

  Millis millisNow();

  class Timer {
  public:
    bool set(float secs);
    bool wait();
    bool clear();
  private:
    Millis time_ = 0;
    Millis start_ = 0;
  };
}

#endif
