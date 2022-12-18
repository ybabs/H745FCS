#ifndef MADGWICK_H
#define MADGWICK_H
/*
 * Implementation of Mahony and Madgwick filters borrowed from https://github.com/sparkfun/SparkFun_MPU-9250_Breakout_Arduino_Library/
 * and https://github.com/kriswiner/MPU9250/
 */

#include "common.h"
#include "math.h"

typedef struct madgwick_filter_t
{
  float roll;
  float pitch;
  float yaw;
  float dt;

}madgwick_filter_t;



int MadgwickQuaternionUpdate(madgwick_filter_t* filter,struct accelData *acc, struct gyroData *gyro, struct magData *mag);
int  MahonyQuaternionUpdate(madgwick_filter_t* filter,struct accelData *acc, struct gyroData *gyro, struct magData *mag);
void MadgwickInit();

//const float* getQ();
const struct quaternion* getQ();

#endif //MADGWICK_H
