#include "mcp23017stepper.h"
//#include "pcf8574stepper.h"
//#include "arduinostepper.h"
//#include "shiftregstepper.h"
#include <Wire.h>
#include <AccelStepper.h>

// Make a Mcp23017StepperControl on i2c address 0x20
//Pcf8574StepperControl pcf8574;
Mcp23017StepperControl mcp23017(0x20);
Mcp23017StepperControl mcp23017_2(0x21);

//ArduinoStepperControl arduino(10, 11, 12, 13);

// Make a ShiftregStepperControl on pins 10, 11, 12
// (ser, latch, clock)
//ShiftregStepperControl shift_reg(10, 11, 12);

Stepper stp1(mcp23017);
Stepper stp2(mcp23017);
Stepper stp3(mcp23017);
Stepper stp4(mcp23017);
Stepper stp5(mcp23017_2);
Stepper stp6(mcp23017_2);
Stepper stp7(mcp23017_2);

// For the AccelSteppers, define two functions to forward
// and backward step a Stepper
void forward(Stepper& stepper) {
  stepper.direction(Stepper::Direction::FORWARD);
  stepper.speed(0);
  stepper.step(1);
}

void backward(Stepper& stepper) {
  stepper.direction(Stepper::Direction::BACKWARD);
  stepper.speed(0);
  stepper.step(1);
}

// Turn Steppers stp1 and stp2 into AccelSteppers using the two functions
AccelStepper stepper1([&](){forward(stp1);}, [&](){backward(stp1);});
AccelStepper stepper2([&](){forward(stp2);}, [&](){backward(stp2);});
AccelStepper stepper3([&](){forward(stp7);}, [&](){backward(stp7);});

void setup() {
  // Initialize libraries
  Wire.begin();

  // Initialize the stepper motor controllers
  mcp23017.begin();
  mcp23017_2.begin();
//  pcf8574.begin();
//  shift_reg.begin();
//  arduino.begin();

  // Initialize and set first targets for the AccelSteppers
  stepper1.setMaxSpeed(500);
  stepper1.setAcceleration(300.0);
  stepper1.moveTo(4096);

  stepper2.setMaxSpeed(600);
  stepper2.setAcceleration(100.0);
  stepper2.moveTo(10000000);

  stepper3.setMaxSpeed(600);
  stepper3.setAcceleration(100.0);
  stepper3.moveTo(10000000);

  // Initialize and set first targets for the two Steppers
  // that are not AccelSteppers
  stp4.turn(1);
  stp4.speed(10);

  stp3.speed(10);
  stp3.start();

  stp5.speed(10);
  stp5.direction(Stepper::Direction::BACKWARD);
  stp5.start();

  stp6.speed(4);
  stp6.direction(Stepper::Direction::BACKWARD);
  stp6.start();

}


void loop() {
  // Update the clock and move the stepper if due
  LoopClock::tick();
  mcp23017.run();
  mcp23017_2.run();
//  pcf8574.run();
//  shift_reg.run();

  // Update the AccelSteppers
  stepper1.run();
  stepper2.run();;
  stepper3.run();;

  // Change direction at the limits
  if (stepper1.distanceToGo() == 0) {
    if (stepper2.isRunning()) {
      stepper2.stop();
      stp3.start();
    }
    else {
      stepper2.move(10000000);
      stp3.stop();
    }
    if (!stp4.running()) {
      stepper1.moveTo(-stepper1.currentPosition());
      stp4.turn(0.5);
      stp4.reverse();
    }
    else {
      stp6.reverse();
    }
    stepper3.moveTo(-stepper3.currentPosition());
    if (stepper3.distanceToGo() == 0) {
      stepper3.moveTo(10000000);
    }
  }
}
