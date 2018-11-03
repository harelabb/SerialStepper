#include "mcp23017stepper.h"
#include <Wire.h>

Mcp23017StepperControl::Mcp23017StepperControl(int i2caddr)
  : i2caddr_(i2caddr) {}

void
Mcp23017StepperControl::begin() {
  Wire.beginTransmission(i2caddr_);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // set all of port A to outputs
  Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();
}

void
Mcp23017StepperControl::doMove(Status status) {
  Wire.beginTransmission(i2caddr_);
  Wire.write(0x12); // GPIOA
  Wire.write(status & 0xFF);
  Wire.write(status >> 8);
  Wire.endTransmission();
}
