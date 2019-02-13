#ifndef SERIALSTEPPER_H
#define SERIALSTEPPER_H

#include <Arduino.h>
#include "loopclock.h"

class StepperControl;

class Stepper {
public:
  enum Direction:byte {BACKWARD = false, FORWARD = true};
  explicit Stepper(StepperControl& control);
  void start();
  void stop();
  void reverse();
  void direction(Direction direction);
  void steps(uint32_t nsteps);

  void speed(float rpm);
  void turn(float turns);

  Direction direction() const;
  uint32_t steps() const;
  bool running() const;

  byte tick();
  void advance(Direction direction);

  Stepper& operator=(const Stepper&) = delete;
  Stepper(const Stepper&) = delete;
  Stepper(Stepper&&) = delete;

private:
  byte pos_ = 0;
  byte status_ = 0;
  Direction direction_ = FORWARD;
  bool reverse_ = false;

  uint32_t micros_pr_step_ = 0;
  uint32_t steps_ = 0;
  loopClock::Micros clock_;
  static constexpr uint32_t steps_pr_turn_ = 2048;
  static constexpr uint32_t infinite_ = ~uint32_t(0);
};

inline Stepper::Direction operator!(Stepper::Direction direction) {
  return direction == Stepper::FORWARD ?
    Stepper::BACKWARD : Stepper::FORWARD;
}

#endif
