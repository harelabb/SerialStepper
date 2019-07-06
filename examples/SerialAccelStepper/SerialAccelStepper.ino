/// @file SerialAccelStepper.ino

/// This example demostrates how to use AccelStepper
/// in combination with SerialStepper.
/// Four AccelSteppers are running.

#include <SerialStepper.h>

//#include <pcf8574stepper.h>
#include <mcp23017stepper.h>

#include <AccelStepper.h>

#include <Wire.h>  // I²C

/// Define the motors
//Pcf8574StepperControl stepper_ctl_0(0x38);
//Pcf8574StepperControl stepper_ctl_1(0x39);
Mcp23017StepperControl stepper_ctl;

/// AccelStepper forward interface
void forward(Stepper& stepper) {
  stepper.speed(0);
  stepper.steps(1);
  stepper.advance(Stepper::Direction::FORWARD);
}

/// AccelStepper backward interface
void backward(Stepper& stepper) {
  stepper.speed(0);
  stepper.steps(1);
  stepper.advance(Stepper::Direction::BACKWARD);
}

/// Define the Steppers to use
Stepper stp[4];

/// Define four AccelSteppers
AccelStepper stepper1{[&](){forward(stp[0]);},
                      [&](){backward(stp[0]);}};
AccelStepper stepper2{[&](){forward(stp[1]);},
                      [&](){backward(stp[1]);}};
AccelStepper stepper3{[&](){forward(stp[2]);},
                      [&](){backward(stp[2]);}};
AccelStepper stepper4{[&](){forward(stp[3]);},
                      [&](){backward(stp[3]);}};

/// This is called once at start-up.
void setup() {

  /// Initialize libraries.
  Wire.begin();

  /// Initialize the stepper motor controller
  stepper_ctl.begin();

  // for (int i = 0; i < 2; ++i) {
  //   stepper_ctl_0.addStepper(stp[i]);
  //   stepper_ctl_1.addStepper(stp[i+2]);
  // }
  for (auto& s : stp) {
    stepper_ctl.addStepper(s);
  }

  /// Initialize AccelStepper speed, acceleration
  /// and distance to go.
  stepper1.setMaxSpeed(300.0);
  stepper1.setAcceleration(100.0);
  stepper1.moveTo(1024);

  stepper2.setMaxSpeed(600.0);
  stepper2.setAcceleration(10.0);
  stepper2.moveTo(-2048);

  stepper3.setMaxSpeed(500.0);
  stepper3.setAcceleration(100.0);
  stepper3.moveTo(2048);

  stepper4.setMaxSpeed(400.0);
  stepper4.setAcceleration(100.0);
  stepper4.moveTo(-1024);
}

/// This is called repeatedly.
void loop() {

  /// Update clock.
  loopClock::tick();

  /// Accelstepper commands
  if (stepper1.distanceToGo() == 0)
    stepper1.moveTo(-stepper1.currentPosition());
  if (stepper2.distanceToGo() == 0)
    stepper2.moveTo(-stepper2.currentPosition());
  if (stepper3.distanceToGo() == 0)
    stepper3.moveTo(-stepper3.currentPosition());
  if (stepper4.distanceToGo() == 0)
    stepper4.moveTo(-stepper4.currentPosition());
  stepper1.run();
  stepper2.run();
  stepper3.run();
  stepper4.run();

  /// Move the steppers if due time.
  stepper_ctl.run();
}
