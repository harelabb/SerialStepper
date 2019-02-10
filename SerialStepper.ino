#include "mcp23017stepper.h"
//#include "pcf8574stepper.h"
//#include "arduinostepper.h"
//#include "shiftregstepper.h"
#include <Wire.h>
#include <AccelStepper.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <Ota.h>

static const char* ssid = "spelemann2";
static const char* password = "konnefin";

// Make a Mcp23017StepperControl on i2c address 0x20
//Pcf8574StepperControl pcf8574;
Mcp23017StepperControl mcp23017(0x20);
//Mcp23017StepperControl mcp23017_2(0x21);

//ArduinoStepperControl arduino(10, 11, 12, 13);

// Make a ShiftregStepperControl on pins 10, 11, 12
// (ser, latch, clock)
//ShiftregStepperControl shift_reg(10, 11, 12);

loopClock::Timer clk1, clk2, clk3;

Stepper stp1(mcp23017);
Stepper stp2(mcp23017);
Stepper stp3(mcp23017);
Stepper stp4(mcp23017);
// Stepper stp5(mcp23017_2);
// Stepper stp6(mcp23017_2);
// Stepper stp7(mcp23017_2);

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

// // Turn Steppers stp1 and stp2 into AccelSteppers using the two functions
// AccelStepper stepper1([&](){forward(stp1);}, [&](){backward(stp1);});
// AccelStepper stepper2([&](){forward(stp2);}, [&](){backward(stp2);});
// AccelStepper stepper3([&](){forward(stp7);}, [&](){backward(stp7);});

void setup() {
  // Initialize libraries
  Serial.begin(115200);
  WifiConnect(ssid, password);
  OtaSetup();
  Wire.begin(0, 2);

  // Initialize the stepper motor controllers
  mcp23017.begin();
//  mcp23017_2.begin();
//  pcf8574.begin();
//  shift_reg.begin();
//  arduino.begin();

  // Initialize and set first targets for the AccelSteppers
  // stepper1.setMaxSpeed(500);
  // stepper1.setAcceleration(300.0);
  // stepper1.moveTo(4096);

  // stepper2.setMaxSpeed(600);
  // stepper2.setAcceleration(100.0);
  // stepper2.moveTo(10000000);

  // stepper3.setMaxSpeed(600);
  // stepper3.setAcceleration(100.0);
  // stepper3.moveTo(10000000);

  // stp1.speed(4);
  // stp2.speed(8);
  // stp4.speed(0);
  // stp3.speed(10);
  stp3.start();
  stp4.start();
  stp2.direction(Stepper::Direction::BACKWARD);
  Serial.println("Starting stepper motors");
}

void runner() {
  loopClock::tick();
  mcp23017.run();
  // Update the clock and move the stepper if due
//  mcp23017_2.run();
//  pcf8574.run();
//  shift_reg.run();

  // Update the AccelSteppers
  // stepper1.run();
  // stepper2.run();
  // stepper3.run();

  // check for ota sketch update
  OtaLoop();
}


void loop() {
  runner();
  if (!stp1.running()) {
    clk1.set(5);
    if (clk1.wait()) {
      stp3.speed(0.5);
      if (stp2.running()) {
        stp1.reverse();
      }
      stp1.speed(4);
      stp1.turn(1.5);
    }
  }
  if (!stp2.running()) {
    clk2.set(6);
    stp2.speed(8);
    stp2.reverse();
    stp2.turn(2);
  }
  if (clk2.wait()) {
    stp3.speed(12);
    stp3.reverse();
  }
  // Ten seconds on, five seconds off
  if (!stp4.running()) {
    if (clk3.wait()) {
      stp4.speed(10);
      stp4.start();
    }
    clk3.set(10);
  }
  else {
    if (clk3.wait()) {
      stp4.speed(0);
    }
    clk3.set(5);
  }
}
