/*
 * File: LEDRing.cpp
 * Author: Raghava Kumar
 */

#include "LEDRing.h"

void LEDRing::solid(CRGB color, int led)
{
  if (led == -1)
  {
    fill_solid(leds_, count_, color);
  }
  else
  {
    leds_[led] = color;
  }
  FastLED.show();
}

void LEDRing::flash(CRGB color, int duration)
{
  solid(color);
  delay(duration);
  solid(CRGB::Black);
}

void LEDRing::pulse(CRGB color, int duration)
{
  CHSV color_hsv = rgb2hsv_approximate(color);
  uint8_t original_v = color_hsv.v;

  int step_duration = (int) (((float) duration)/(2.0*(original_v+1)));

  for (int i = 0; i <= original_v; i++)
  {
    color_hsv.v = i;
    solid(color_hsv);
    delay(step_duration);
  }

  for (int i = original_v; i >= 0; i--)
  {
    color_hsv.v = i;
    solid(color_hsv);
    delay(step_duration);
  }
}

void LEDRing::run(CRGB color, int repetitions, int duration, bool direction)
{
  for (int i = 0; i < repetitions; i++)
  {
    if (direction)
    {
      for (int j = 0; j < count_; j++)
      {
        solid(color, j);
        delay(duration);
        solid(CRGB::Black, j);
      }
    }
    else
    {
      for (int j = count_-1; j >= 0; j--)
      {
        solid(color, j);
        delay(duration);
        solid(CRGB::Black, j);
      } 
    }
  }
}

void LEDRing::fatalError(int duration)
{
  uint64_t start = millis();
  while(millis() - start < duration)
  {
    flash(CRGB::Red, 100);
    delay(200);
  }

  while(true);
}
