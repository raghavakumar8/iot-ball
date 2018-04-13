/*
 * File: websocket.cpp
 * Author: Raghava Kumar
 * Reference: https://platformio.org/lib/show/549/WebSockets/examples
 */

#include "scrum-tracker/include/scrum_tracker.h"

MPU9250_DMP imu;

void setupIMU()
{
  if (imu.begin() != INV_SUCCESS)
  {
    Serial.println("IMU not found. Please check your wires!");
    flashFatalError();
  }

  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
  imu.setGyroFSR(2000);
  imu.setAccelFSR(2);
  imu.setLPF(5);

  imu.setSampleRate(10);

  Serial.println("IMU setup successfully!");
}

void updateIMU()
{
  if (imu.dataReady())
  {
    imu.update();

    float accelX = imu.calcAccel(imu.ax);
    float accelY = imu.calcAccel(imu.ay);
    float accelZ = imu.calcAccel(imu.az);

    float gyroX = imu.calcGyro(imu.gx);
    float gyroY = imu.calcGyro(imu.gy);
    float gyroZ = imu.calcGyro(imu.gz);

    Serial.printf("Accel: %f %f %f\n", accelX, accelY, accelZ);
    Serial.printf("Gyro: %f %f %f\n", gyroX, gyroY, gyroZ);
    Serial.println("-------");
  }
  delay(10);
}

bool throwDetected()
{
  return false;
}

bool shakeDetected()
{
  return false;
}
