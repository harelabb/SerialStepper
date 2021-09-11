#include "unostepper.h"
#include <Arduino.h>

UnoStepperControl::UnoStepperControl() {}

#ifdef ARDUINO_AVR_UNO

void
UnoStepperControl::doBegin() {
  static constexpr uint8_t output = ~0;
  DDRB = 0x0F; // pins 8, 9,10,11
  DDRD = 0xF0; // pins 4, 5, 6, 7

  PORTD = 0;
  PORTB = 0;
}

void
UnoStepperControl::doMove(Status status) {
  PORTD = status & 0xF0;
  PORTB = status & 0x0F;
}

#else
void UnoStepperControl::doBegin() {}
void UnoStepperControl::doMove(Status) {}
#endif
