#include "megastepper.h"
#include <Arduino.h>

MegaStepperControl::MegaStepperControl() {}

#ifdef ARDUINO_AVR_MEGA2560

void
MegaStepperControl::doBegin() {
  static constexpr uint8_t output = ~0;
  DDRA = output;
  DDRC = output;
  DDRL = output;
  DDRB = output;
  DDRF = output;
  DDRK = output;

  PORTA = 0;
  PORTC = 0;
  PORTL = 0;
  PORTB = 0;
  PORTF = 0;
  PORTK = 0;
}

void
MegaStepperControl::doMove(Status status) {
  PORTA = (status >>  0) & 0xFF;
  PORTC = (status >>  8) & 0xFF;
  PORTL = (status >> 16) & 0xFF;
  PORTB = (status >> 24) & 0xFF;
  PORTF = (status >> 32) & 0xFF;
  PORTK = (status >> 40) & 0xFF;
}

#else
void MegaStepperControl::doBegin() {}
void MegaStepperControl::doMove(Status) {}
#endif
