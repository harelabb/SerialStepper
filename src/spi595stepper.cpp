#include "spi595stepper.h"
#include <Arduino.h>
#include <SPI.h>

Spi595StepperControl::Spi595StepperControl(byte ltc_pin)
  : ltc_pin_(ltc_pin) {}

void
Spi595StepperControl::doBegin() {
  pinMode(ltc_pin_, OUTPUT);
}

void
Spi595StepperControl::doMove(Status status) {
  constexpr int MHz = 1000000;
  constexpr int speed = 20 * MHz;
  digitalWrite(ltc_pin_, LOW);
  SPI.beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));
  int nbytes = (nSteppers() + 1) / 2;
  while (nbytes) {
    SPI.transfer((status >> (--nbytes * 8)) & 0xFF);
  }
  SPI.endTransaction();
  digitalWrite(ltc_pin_, HIGH);
}
