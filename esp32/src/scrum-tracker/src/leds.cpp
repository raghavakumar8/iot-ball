/*
 * File: websocket.cpp
 * Author: Raghava Kumar
 * Reference: https://platformio.org/lib/show/549/WebSockets/examples
 */

#include "scrum-tracker/include/scrum_tracker.h"

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN);

void setupLEDS()
{
  leds.begin();
  leds.show();
}

void setColor(uint8_t r, uint8_t g, uint8_t b)
{
  for (int i = 0; i < NEOPIXEL_COUNT; i++)
  {
    leds.setPixelColor(i, r, g, b);
  }
  leds.show();
}

void flash(uint8_t r, uint8_t g, uint8_t b, int ms)
{
  setColor(r, g, b);
  delay(ms);
  setColor(0, 0, 0);
}

void pulsate(uint8_t r, uint8_t g, uint8_t b, int ms)
{
  static int steps = 20;
  int animation_delay = (int) (((float) ms)/(2*steps));
  for (int i = 0; i < steps; i++)
  {
    float multiplier = ((float) i)/steps;
    setColor(r*multiplier, g*multiplier, b*multiplier);
    delay(animation_delay);
  }

  for (float i = steps; i > 0; i--)
  {
    float multiplier = ((float) i)/steps;
    setColor(r*multiplier, g*multiplier, b*multiplier);
    delay(animation_delay);
  }
  
}

void circle(uint8_t r, uint8_t g, uint8_t b, int times, int ms, bool direction)
{
  for (int j = 0; j < times; j++)
  {
    if (direction)
    {
      for (int i = 0; i < NEOPIXEL_COUNT; i++)
      {
        leds.setPixelColor(i, r, g, b);
        leds.show();
        delay(ms);
        leds.setPixelColor(i, 0);
        leds.show();
      }
    }
    else
    {
      for (int i = NEOPIXEL_COUNT-1; i >= 0; i--)
      {
        leds.setPixelColor(i, r, g, b);
        leds.show();
        delay(ms);
        leds.setPixelColor(i, 0);
        leds.show();
      } 
    }
  }
}