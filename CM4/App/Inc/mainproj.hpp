#pragma once

#include <bmp280.hpp>
#include <ublox.hpp>
#include <lsm9ds1.hpp>



class EntryApp
{

public:

  explicit EntryApp();
  void BaroTask();
  void GPSTask();
  void AccelTask();
  void MagTask();
  void GyroTask();
  void RunSensors();


private:

  Bmp280 baro;
  GPS gps;
  IMU imu;
  /* Shared Address spaces for M4 and M7 in D3 Region */
  volatile struct accelData *acc_values_m4 = (struct accelData*) ACCEL_D3_ADDRESS;
  volatile struct gyroData *gyro_values_m4 = (struct gyroData*) GYRO_D3_ADDRESS;
  volatile struct magData *mag_values_m4 = (struct magData*) MAG_D3_ADDRESS;
  volatile struct baroData *baro_values_m4 = (struct baroData*) BARO_D3_ADDRESS;
  volatile struct gpsData *gps_values_m4 = (struct gpsData*) GPS_D3_ADDRESS;



};






