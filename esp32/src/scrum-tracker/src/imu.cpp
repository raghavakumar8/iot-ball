/*
 * File: websocket.cpp
 * Author: Raghava Kumar
 * Reference: https://platformio.org/lib/show/549/WebSockets/examples
 */

#include "scrum-tracker/include/scrum_tracker.h"

MPU9250 imu;

void setupIMU()
{
  Wire.begin();
  uint8_t c = imu.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);

  if (c != 0x71)
  {
    Serial.println("IMU not found. Please check your wires!");
    flashFatalError();
  }

  Serial.println("IMU setup successfully!");
}

void update()
{
}

bool throwDetected()
{
  return false;
}

bool shakeDetected()
{
  return false;
}
