#pragma once
#include <FastLED.h>
#include <vector>

template<template<fl::u8 DATA_PIN, EOrder RGB_ORDER> class LEDType, fl::u8 Pin>
class LedManager
{
public:
  LedManager() = default;
  ~LedManager() = default;

  inline CRGBSet getView() {return CRGBSet(_leds.data(), _leds.size());}

  inline void clearLeds(){_leds.clear();}

  inline void setLed(int idx, uint8_t r, uint8_t g, uint8_t b)
  {
    if(idx >= 0 && idx < _leds.size()) _leds[idx] = CRGB(r, g, b);
  }

  inline CRGB getLed(int idx)
  {
    if(idx >= 0 && idx < _leds.size()) return _leds[idx];
    return CRGB::Black;
  }

  inline void addLeds(int amount)
  {
    for(int i = 0; i < amount; i++)
    {
      _leds.push_back({0});
    }
    FastLED.addLeds<LEDType, Pin>(_leds.data(), _leds.size());
  }

  inline int ledCount() {return _leds.size();}

private:
  std::vector<CRGB> _leds;
};