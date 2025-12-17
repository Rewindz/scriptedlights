#pragma once

#include <Arduino.h>
#include <SPI.h>

class EEPROMManager
{
public:

  static constexpr int8_t MSB = MSBFIRST;
  static constexpr int8_t LSB = LSBFIRST;

  ~EEPROMManager();
  EEPROMManager(uint8_t csPin, uint32_t clock, uint8_t bitOrder, uint8_t dataMode);
  EEPROMManager() = delete;

  void begin();
  void end();

  void transfer(size_t sz, void *data);


private:

  inline void _write(uint8_t val) {digitalWrite(_csPin, val);}

  uint8_t _csPin;
  SPISettings _spiSettings;
};