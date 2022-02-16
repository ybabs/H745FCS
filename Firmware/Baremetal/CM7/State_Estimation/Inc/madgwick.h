#ifndef MADGWICK_H
#define MADGWICK_H
/*
 * Implementation of Mahony and Madgwick filters borrowed from https://github.com/sparkfun/SparkFun_MPU-9250_Breakout_Arduino_Library/
 * and https://github.com/kriswiner/MPU9250/
 */

#include "common.h"
#include "math.h"

struct madgwick_filter_t
{

};

struct mahony_filter_t
{

};

int MadgwickQuaternionUpdate(struct acc_data *acc, struct gyro_data *gyro, struct mag_data *mag, float dt);
int MahonyQuaternionUpdate(struct acc_data *acc, struct gyro_data *gyro, struct mag_data *mag, float dt);

const float* getQ();

#endif
