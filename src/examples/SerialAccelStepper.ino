#include "SerialStepper.h"
#include "pcf8574stepper.h"

#include <AccelStepper.h>

#include <Wire.h>         // I²C

// Make a two Pcf8574StepperControls (each can have two steppers)
Pcf8574StepperControl stepper_ctl_0(0x38);
Pcf8574StepperControl stepper_ctl_1(0x39);

// Define four stepper motors
Stepper stp0(stepper_ctl_0);
Stepper stp1(stepper_ctl_0);
Stepper stp2(stepper_ctl_1);
Stepper stp3(stepper_ctl_1);

// Define AccelStepper interface
void forward(Stepper& stepper) {
  stepper.speed(0);
  stepper.steps(1);
  stepper.advance(Stepper::Direction::FORWARD);
}
void backward(Stepper& stepper) {
  stepper.speed(0);
  stepper.steps(1);
  stepper.advance(Stepper::Direction::BACKWARD);
}

// Turn the Steppers into AccelSteppers using the two functions
AccelStepper stepper1([&](){forward(stp0);}, [&](){backward(stp0);});
AccelStepper stepper2([&](){forward(stp1);}, [&](){backward(stp1);});
AccelStepper stepper3([&](){forward(stp2);}, [&](){backward(stp2);});
AccelStepper stepper4([&](){forward(stp3);}, [&](){backward(stp3);});

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize the stepper motor controller
  stepper_ctl.begin();

  stepper1.setMaxSpeed(600.0);
  stepper1.setAcceleration(100.0);
  stepper1.moveTo(2048);
}

void loop() {
  loopClock::tick();

  // AccelStepper commands
  if (stepper1.distanceToGo() == 0) {
    stepper1.moveTo(-stepper1.currentPosition());
  }
  stepper1.run();
  stepper2.run();
  stepper3.run();
  stepper4.run();

  // Move the steppers if due time
  stepper_ctl_0.run();
  stepper_ctl_1.run();
}
