/**
 * File: blink.cpp
 * Author: Raghava Kumar
 */

#include <Arduino.h>
#include <MPU9250.h>

MPU9250 myIMU;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
}

void loop()
{
  byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.print(F("MPU9250 I AM 0x"));
  Serial.print(c, HEX);
  Serial.print(F(" I should be 0x"));
  Serial.println(0x71, HEX);
}
 