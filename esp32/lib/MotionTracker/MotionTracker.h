/*
 * File: MotionTracker.h
 * Author: Raghava Kumar
 */

#ifndef MOTION_TRACKER_H
#define MOTION_TRACKER_H

#include <SparkFunMPU9250-DMP.h>

enum class Motion {NONE, THROW, SHAKE, TWIST};

class MotionTracker
{
public:
  MotionTracker(){}
  ~MotionTracker(){}

  bool initialize();
  Motion detect();

private:
  void updateData();

  enum Primitive {MEH, LOW_ACCEL, VERY_HIGH_ACCEL, HIGH_GYRO};
  MotionTracker::Primitive buffer_[5];
  int buffer_pos_ = 0;

  uint64_t last_motion_time_;
  uint64_t last_update_time_;

  float accel_[3] = {0.0, 0.0, 0.0};
  float gyro_[3] = {0.0, 0.0, 0.0};
  float total_accel_ = 0.0;

  MPU9250_DMP imu_;
};

#endif // MOTION_TRACKER_H