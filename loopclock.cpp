#include "loopclock.h"
#include <Arduino.h>

namespace {
  using MsTime = decltype(::millis());
  MsTime ms_now_ = ::millis();
  LoopClock::Time now_ =  ::micros();
}

namespace LoopClock {

  void tick() {
    ms_now_ = ::millis();
    now_ = ::micros();
  }

  Time now() {
    return now_;
  }

  bool Timer::wait(float secs) {
    if (secs_ > 0) {
      if (secs_ < ms_now_ / 1000) {
        secs_ = 0;
        return true;
      }
    }
    else {
      secs_ = secs + ms_now_ / 1000;
    }
    return false;
  }
}
