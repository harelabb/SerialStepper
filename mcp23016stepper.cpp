#include "mcp23016stepper.h"
#include <Wire.h>

Mcp23016StepperControl::Mcp23016StepperControl(int i2caddr)
  : i2caddr_(i2caddr) {}

void
Mcp23016StepperControl::begin() {
  Wire.beginTransmission(i2caddr_);
  Wire.write(0x06); // IODIRA register
  Wire.write(0x00); // set all of port A to outputs
  Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();
}

void
Mcp23016StepperControl::doTick(Status status) {
  Wire.beginTransmission(i2caddr_);
  Wire.write(0x00); // GPIOA
  Wire.write(status & 0xFF);
  Wire.write(status >> 8);
  Wire.endTransmission();
}
