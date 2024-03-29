#include "mcp23s17stepper.h"
#include <SPI.h>

namespace {
  constexpr uint8_t OPCODEW = 0x40;
  constexpr uint8_t GPIOA = 0x12;
  constexpr uint8_t HAEN = 0x8;
  constexpr uint8_t IOCON = 0xA;


  void beginTransaction(uint8_t cs_pin) {
    constexpr auto MHz = 1000000;
    constexpr auto speed = 20 * MHz;
    digitalWrite(cs_pin, LOW);
    SPI.beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));
  }

  void endTransaction(uint8_t cs_pin) {
    SPI.endTransaction();
    digitalWrite(cs_pin, HIGH);
  }

}


void
Mcp23S17StepperControl::doMove(Status status) {
  beginTransaction(cs_pin_);
  SPI.transfer(OPCODEW | (addr_ << 1));
  SPI.transfer(GPIOA);
  SPI.transfer(status >> 8);
  SPI.transfer(status & 0xFF);
  endTransaction(cs_pin_);
}


void
Mcp23S17StepperControl::doBegin() {
  pinMode(cs_pin_, OUTPUT);
  digitalWrite(cs_pin_, HIGH);

  beginTransaction(cs_pin_);
  SPI.transfer(OPCODEW | (addr_ << 1));
  SPI.transfer(IOCON);
  SPI.transfer(HAEN);
  endTransaction(cs_pin_);

  beginTransaction(cs_pin_);
  SPI.transfer(OPCODEW | (addr_ << 1));
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  endTransaction(cs_pin_);
}

Mcp23S17StepperControl::Mcp23S17StepperControl(uint8_t cs_pin,
                                               uint8_t addr)
  : cs_pin_(cs_pin), addr_(addr) {}
