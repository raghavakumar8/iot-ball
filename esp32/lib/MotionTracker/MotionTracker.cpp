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

  last_motion_time_ = millis();

  Serial.println("IMU setup successfully!");
  return true;
}

Motion MotionTracker::detect()
{
  if (imu_.dataReady())
  {
    imu_.update();
    updateData();
    
    // Process buffer into histogram
    int count[4] = {0, 0, 0, 0};
    for (int i = buffer_pos_; i < buffer_pos_ + 5; i++)
    {
      count[buffer_[i % 5]]++;
    }

    // Detect motion!
    Motion detected = Motion::NONE;
    uint64_t now = millis();

    if (now - last_motion_time_ > 1000)
    {
      // Throw?
      if (buffer_[buffer_pos_ % 5] == LOW_ACCEL && count[LOW_ACCEL] >= 3)
      {
        detected = Motion::THROW;
        last_motion_time_ = now;

        Serial.println("Detected throw!");
      }

      // Twist?
      if (buffer_[buffer_pos_ % 5] == HIGH_GYRO && count[HIGH_GYRO] >= 2 &&
          count[LOW_ACCEL] == 0 && count[VERY_HIGH_ACCEL] == 0 &&
          abs(abs(accel_[2]) - 1.0) < 0.15)
      {
        detected = Motion::TWIST;
        last_motion_time_ = now;

        Serial.println("Detected twist!");
      }

      // Shake?
      if (buffer_[buffer_pos_ % 5] == VERY_HIGH_ACCEL && count[VERY_HIGH_ACCEL] >= 2
          && count[LOW_ACCEL] == 0)
      {
        detected = Motion::SHAKE;
        last_motion_time_ = now;

        Serial.println("Detected shake!");
      }
    }

    return detected;
  }
  else
  {
    return Motion::NONE;
  }
}

void MotionTracker::updateData()
{
  // Update raw values
  accel_[0] = imu_.calcAccel(imu_.ax);
  accel_[1] = imu_.calcAccel(imu_.ay);
  accel_[2] = imu_.calcAccel(imu_.az);

  total_accel_ = accel_[0]*accel_[0] + \
                 accel_[1]*accel_[1] + \
                 accel_[2]*accel_[2];

  gyro_[0] = imu_.calcGyro(imu_.gx);
  gyro_[1] = imu_.calcGyro(imu_.gy);
  gyro_[2] = imu_.calcGyro(imu_.gz);

  // Update measurement buffer
  if(total_accel_ < 0.1)
  {
    buffer_[buffer_pos_++ % 5] = LOW_ACCEL;
  }
  else if(total_accel_ > 3.0)
  {
    buffer_[buffer_pos_++ % 5] = VERY_HIGH_ACCEL;
  }
  else if(abs(gyro_[0]) > 220.0 || 
          abs(gyro_[1]) > 220.0 || 
          abs(gyro_[2]) > 220.0)
  {
    buffer_[buffer_pos_++ % 5] = HIGH_GYRO;
  }
  else
  {
    buffer_[buffer_pos_++ % 5] = MEH;
  }    
}
