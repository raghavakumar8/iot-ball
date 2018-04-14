/*
 * File: MotionTracker.h
 * Author: Raghava Kumar
 */

#ifndef MOTION_TRACKER_H
#define MOTION_TRACKER_H

#include <SparkFunMPU9250-DMP.h>

enum class Motion{NONE, THROW, JERK, SHAKE, TWIST, TAP};

class MotionTracker
{
public:
  MotionTracker(){}
  ~MotionTracker(){}

  bool initialize();
  Motion detect();

private:
  void updateData();
  
  float accel_[3] = {0.0, 0.0, 0.0};
  float gyro_[3] = {0.0, 0.0, 0.0};
  float total_accel_ = 0.0;

  Motion last_detected_ = Motion::NONE;
  MPU9250_DMP imu_;
};

#endif // MOTION_TRACKER_H