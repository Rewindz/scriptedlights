#include "EEPROMManager.h"


EEPROMManager::EEPROMManager(uint8_t csPin, uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
  : _spiSettings(clock, bitOrder, dataMode), _csPin(csPin)
{
  pinMode(_csPin, OUTPUT);
  _write(HIGH);
}

EEPROMManager::~EEPROMManager()
{
  
}

void EEPROMManager::end()
{
  _write(HIGH);
  SPI.endTransaction();
}

void EEPROMManager::begin()
{
  SPI.beginTransaction(_spiSettings);
  _write(LOW);
}

void EEPROMManager::transfer(size_t sz, void *data)
{
  SPI.transfer(data, sz);
}