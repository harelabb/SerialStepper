#include "loopclock.h"
#include <Arduino.h>

namespace {

  loopClock::Millis millis_ = ::millis();
  loopClock::Micros micros_ = ::micros();

}


namespace loopClock {

  void tick() {
    millis_ = ::millis();
    micros_ = ::micros();
  }

  Micros now() {
    return micros_;
  }

  bool Timer::set(float secs) {
    if (time_ > 0) {
      return false;
    }
    time_ = Millis(secs * 1000 + millis_);
    return true;
  }

  bool Timer::wait() {
    if (time_ > 0 && time_ < millis_) {
      time_ = 0;
      return true;
    }
    return false;
  }

}
