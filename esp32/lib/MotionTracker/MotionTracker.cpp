/*
 * File: MotionTracker.cpp
 * Author: Raghava Kumar
 */

#include "MotionTracker.h"

bool MotionTracker::initialize()
{
  if (imu_.begin() != INV_SUCCESS)
  {
    Serial.println("IMU not found. Please check your wires!");
    return false;
  }

  imu_.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
  imu_.setGyroFSR(2000);
  imu_.setAccelFSR(2);
  imu_.setLPF(5);

  imu_.setSampleRate(10);

  Serial.println("IMU setup successfully!");
  return true;
}

Motion MotionTracker::detect()
{
  if (imu_.dataReady())
  {
    imu_.update();
    updateData();
    
    // Process data here

    Serial.printf("Accel: %f \n", total_accel_);
    Serial.printf("Accel: %f %f %f\n", accel_[0], accel_[1], accel_[2]);
    Serial.printf("Gyro: %f %f %f\n", gyro_[0], gyro_[1], gyro_[2]);
    Serial.println("-------");

    return Motion::NONE;
  }
  else
  {
    return Motion::NONE;
  }
}

void MotionTracker::updateData()
{
  accel_[0] = imu_.calcAccel(imu_.ax);
  accel_[1] = imu_.calcAccel(imu_.ay);
  accel_[2] = imu_.calcAccel(imu_.az);

  total_accel_ = accel_[0]*accel_[0] + \
                 accel_[1]*accel_[1] + \
                 accel_[2]*accel_[2];

  gyro_[0] = imu_.calcGyro(imu_.gx);
  gyro_[1] = imu_.calcGyro(imu_.gy);
  gyro_[2] = imu_.calcGyro(imu_.gz);
}
