#include "mcp23016stepper.h"
#include "shiftregstepper.h"
#include <Wire.h>
#include <AccelStepper.h>

// Make a Mcp23016StepperControl on i2c address 0x20
Mcp23016StepperControl expander(0x20);

// Make a ShiftregStepperControl on pins 10, 11, 12
//ShiftregStepperControl shift_reg(10, 11, 12);

// Make two Mcp23016steppers at unit 1 and 3 of the contoller expander
Stepper stp1(expander, 1);
Stepper stp3(expander, 3);
Stepper stp2(expander, 0);
Stepper stp4(expander, 2);

// Make two ShiftregSteppers at unit 1 and 0 of the contoller shift_reg
// Stepper stp2(shift_reg, 1);
// Stepper stp4(shift_reg, 0);

// For the AccelSteppers, define two functions to forward
// and backward step a Stepper
void fwd(Stepper& stp) {
  stp.forward(true);
  stp.speed(0);
  stp.steps(1);
}

void bwd(Stepper& stp) {
  stp.forward(false);
  stp.speed(0);
  stp.steps(1);
}

// Turn Steppers stp1 and stp2 into AccelSteppers using the two functions
AccelStepper stepper1([&](){fwd(stp1);}, [&](){bwd(stp1);});
AccelStepper stepper2([&](){fwd(stp2);}, [&](){bwd(stp2);});


void setup() {
  // Initialize libraries
  Serial.begin(115200);
  Wire.begin();

  // Initialize the stepper motor controllers
  expander.begin();
//  shift_reg.begin();

  // Initialize and set first targets for the AccelSteppers
  stepper1.setMaxSpeed(500);
  stepper1.setAcceleration(300.0);
  stepper1.moveTo(4096);

  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(100.0);
  stepper2.moveTo(10000000);

  // Initialize and set first targets for the two Steppers
  // that are not AccelSteppers
  stp4.turn(1);
  stp4.speed(10);

  stp3.speed(10);
  stp3.run();
  }


void loop() {
  // Update the clock and move the stepper if due
  LoopClock::tick();
  expander.tick();
//  shift_reg.tick();

  // Change direction at the limits
  if (stepper1.distanceToGo() == 0) {
    if (stepper2.isRunning()) {
      stepper2.stop();
      stp3.run();
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
  }
  // Update the AccelSteppers
  stepper1.run();
  stepper2.run();;
}
