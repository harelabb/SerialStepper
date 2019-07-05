#include "shiftout595stepper.h"
#include <Arduino.h>
#include <Wire.h>

Shiftout595StepperControl::
Shiftout595StepperControl(byte ser_pin,
                          byte ltc_pin,
                          byte clk_pin)
  : ser_pin_(ser_pin),
    ltc_pin_(ltc_pin),
    clk_pin_(clk_pin) {}

void
Shiftout595StepperControl::doBegin() {
  pinMode(clk_pin_, OUTPUT);
  pinMode(ser_pin_, OUTPUT);
  pinMode(ltc_pin_, OUTPUT);
}


void
Shiftout595StepperControl::doMove(Status status) {
  digitalWrite(clk_pin_, LOW);
  digitalWrite(ltc_pin_, LOW);
  int nbytes = (nSteppers() + 1) / 2;
  while (nbytes--) {
    shiftOut(ser_pin_, clk_pin_, MSBFIRST,
             status >> (nbytes * 8));
  }
  digitalWrite(ltc_pin_, HIGH);
}
