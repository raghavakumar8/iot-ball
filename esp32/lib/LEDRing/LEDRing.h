/*
 * File: LEDRing.h
 * Author: Raghava Kumar
 */

#ifndef LED_RING_H
#define LED_RING_H

#include <FastLED.h>

class LEDRing
{

public:
  LEDRing(int count)
   : count_(count)
  {
    leds_ = (CRGB*) calloc(count, sizeof(CRGB));
  }

  ~LEDRing()
  {
    free(leds_);
  }

  template <int LED_PIN>
  void initialize()
  {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds_, count_);
    FastLED.show();

    Serial.println("\nLEDs setup successfully!");
  }

  void solid(CRGB color, int led=-1);
  void flash(CRGB color, int duration);
  void pulse(CRGB color, int duration);
  void run(CRGB color, int repetitions, int duration, bool direction);
  void fatalError(int duration);

private:
  CRGB* leds_;
  int count_;
};

#endif // LED_RING_H