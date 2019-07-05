#include "pcf8574stepper.h"
#include <Wire.h>

Pcf8574StepperControl::Pcf8574StepperControl(int i2caddr)
  : i2caddr_(i2caddr) {}

void
Pcf8574StepperControl::doBegin() {
  Wire.beginTransmission(i2caddr_);
  Wire.write(0xFF);
  Wire.endTransmission();
}

void
Pcf8574StepperControl::doMove(Status status) {
  Wire.beginTransmission(i2caddr_);
  Wire.write(status & 0xFF);
  Wire.endTransmission();
}
