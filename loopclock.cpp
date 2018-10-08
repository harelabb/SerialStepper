#include "loopclock.h"
#include <Arduino.h>

namespace LoopClock {

  static Time now_ =  ::micros();

  void tick() {
    now_ = ::micros();
  }

  Time now() {
    return now_;
  }
}
