/// @file SerialAccelStepper.ino

/// This example demostrates how to use AccelStepper
/// in combination with SerialStepper.
/// Four AccelSteppers are running.
/// \nWe uese two PCF8574A with two steppers each.
/// Remember a 47kΩ pull-up resistor on each i/o pin!

#include <SerialStepper.h>

#include <pcf8574stepper.h>

#include <AccelStepper.h>

#include <Wire.h>  // I²C

/// Define the stepper controllers.
Pcf8574StepperControl stepper_ctl [] {
  Pcf8574StepperControl {0x38},
  Pcf8574StepperControl {0x39}};

/// AccelStepper forward interface.
void forward(Stepper& stepper) {
  stepper.speed(0);
  stepper.steps(1);
  stepper.advance(Stepper::Direction::FORWARD);
}

/// AccelStepper backward interface.
void backward(Stepper& stepper) {
  stepper.speed(0);
  stepper.steps(1);
  stepper.advance(Stepper::Direction::BACKWARD);
}

/// Define the Steppers to use
Stepper stp[4];

/// Define four AccelSteppers
AccelStepper steppers [] {
  AccelStepper {[&](){forward (stp[0]);},
                [&](){backward(stp[0]);}},
  AccelStepper {[&](){forward (stp[1]);},
                [&](){backward(stp[1]);}},
  AccelStepper {[&](){forward (stp[2]);},
                [&](){backward(stp[2]);}},
  AccelStepper {[&](){forward (stp[3]);},
                [&](){backward(stp[3]);}}};

/// This is called once at start-up.
void setup() {

  /// Initialize libraries.
  Wire.begin();

  /// Initialize the stepper motor controllers
  for (auto& ctl : stepper_ctl) {
    ctl.begin();
  }

  /// Assign steppers to the controllers
  for (int i = 0; i < 2; ++i) {
    stepper_ctl[0].addStepper(stp[i]);
    stepper_ctl[1].addStepper(stp[i+2]);
  }

  /// Initialize AccelStepper speed, acceleration
  /// and distance to go.
  for (int i = 0; i < 4; ++i) {
    steppers[i].setMaxSpeed(500 - i * 100);
    steppers[i].setAcceleration(100.0);
    steppers[i].moveTo(-2048 + i* 1024);
  }
}

/// This is called repeatedly.
void loop() {

  /// Update clock.
  loopClock::tick();

  /// AccelStepper commands as normal
  for (auto& stepper : steppers) {
    if (stepper.distanceToGo() == 0) {
      stepper.moveTo(-stepper.currentPosition());
    }
  }
  /// Update the AccelSteppers as normal
  for (auto& stepper : steppers) {
    stepper.run();
  }

  /// Move the Steppers if due time.
  for (auto& ctl : stepper_ctl) {
    ctl.run();
  }
}
