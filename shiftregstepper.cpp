#include "shiftregstepper.h"
#include <Arduino.h>
#include <Wire.h>

ShiftregStepperControl::
ShiftregStepperControl(byte ser_pin, byte ltc_pin, byte clk_pin)
  : ser_pin_(ser_pin), ltc_pin_(ltc_pin), clk_pin_(clk_pin) {}

void
ShiftregStepperControl::begin() {
  pinMode(clk_pin_, OUTPUT);
  pinMode(ser_pin_, OUTPUT);
  pinMode(ltc_pin_, OUTPUT);
}


void
ShiftregStepperControl::doTick(Status status) {
  digitalWrite(clk_pin_, LOW);
  digitalWrite(ltc_pin_, LOW);
  int i = sizeof(Status);
  while (i--) {
    shiftOut(ser_pin_, clk_pin_, MSBFIRST,
             (status & (0xff << i * 8)) >> (i * 8));
  }
  digitalWrite(ltc_pin_, HIGH);
}
