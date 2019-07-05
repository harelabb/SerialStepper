#ifndef SERIALSTEPPER_H
#define SERIALSTEPPER_H

#include <Arduino.h>
#include "loopclock.h"

/// This class represents a stepper motor.
/// <p>The Stepper will need to be associated with a StepperControl.
/// <p> The sketch using the Stepper should not delay in the loop().
class Stepper {
public:

  /// Stepper directions.
  enum Direction {BACKWARD = false, FORWARD = true};

  /// Constructor, define a Stepper.
  explicit Stepper(uint32_t steps_pr_turn = 2048);

  /// Start running continuously at current speed.
  void start();

  /// Stop running.
  void stop();

  /// Set move direction.
  void direction(Direction direction);

  /// Move nsteps steps at current speed in current direction.
  void steps(uint32_t nsteps);

  /// Set the speed in RPM.
  void speed(float rpm);

  /// Move turns in current speed in current direction.
  void turn(float turns);

  /// Get the current direction.
  Direction direction() const;

  /// Get remaining steps.
  uint32_t steps() const;

  /// Check if running.
  bool running() const;

  /// Get the speed in RPM.
  float speed() const;

  /// Move if due time.
  /// Is called by the StepperControl
  byte tick();

  /// Unconditionally move one step in direction.
  void advance(Direction direction);

private:
  const uint32_t steps_pr_turn_;
  byte pos_ = 0;
  Direction direction_ = FORWARD;

  uint32_t micros_pr_step_ = 0;
  uint32_t steps_ = 0;
  loopClock::Micros clock_;
  static constexpr uint32_t infinite_ = ~uint32_t(0);
};

/// @file

/// Not operator on Direction gives the reverse.
inline Stepper::Direction operator!(Stepper::Direction direction) {
  return direction == Stepper::FORWARD ?
    Stepper::BACKWARD : Stepper::FORWARD;
}
#endif
