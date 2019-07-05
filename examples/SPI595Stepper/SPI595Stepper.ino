/// @file
/// Example of using 74HC595 shift registers to drive 8 stepper motors.
#include <SerialStepper.h>
#include <spi595stepper.h>
#include <SPI.h>

/// Define the number of steppers
constexpr auto nsteppers = 8;

/// Define the pin to use as latch pin
constexpr int latch_pin = 4; // for Attiny85

/// Define the Stepper motors.
Stepper steppers[nsteppers];

/// Define the StepperControl.
Spi595StepperControl stepper_ctl{latch_pin};

/// This is called once at start-up.
void setup() {

  /// Initialize libraries.
  SPI.begin();

  /// Initialize the stepper motor controller.
  stepper_ctl.begin();
  for (auto& stepper : steppers) {
    stepper_ctl.addStepper(stepper);
  }

  /// Set different speed for each Stepper.
  constexpr float max_speed = 15.0f;
  for (int i = 0; i < nsteppers; ++i) {
    steppers[i].speed(max_speed - 1.0f * i);
  }
}

/// This is called repeatedly.
void loop() {

  /// Update the clock.
  loopClock::tick();

  /// Give commands to the steppers.
  for (auto& s : steppers) {
    if (!s.running()) {
      s.direction(!s.direction());
      s.turn(0.5);
    }
  }

  /// Move the steppers if due time.
  stepper_ctl.run();
  yield();
}
