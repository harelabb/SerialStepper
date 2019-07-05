#include "arduinostepper.h"
#include <Arduino.h>

ArduinoStepperControl::
ArduinoStepperControl(byte pin1, byte pin2, byte pin3, byte pin4)
  : pin1_(pin1), pin2_(pin2), pin3_(pin3), pin4_(pin4) {}

void
ArduinoStepperControl::doBegin() {
  pinMode(pin1_, OUTPUT);
  pinMode(pin2_, OUTPUT);
  pinMode(pin3_, OUTPUT);
  pinMode(pin4_, OUTPUT);
}


void
ArduinoStepperControl::doMove(Status status) {
  digitalWrite(pin1_, status & 1 ? HIGH : LOW);
  digitalWrite(pin2_, status & 2 ? HIGH : LOW);
  digitalWrite(pin3_, status & 4 ? HIGH : LOW);
  digitalWrite(pin4_, status & 8 ? HIGH : LOW);
}
