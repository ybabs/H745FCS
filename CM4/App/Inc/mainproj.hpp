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
  void M4DataToM7(const uint8_t data_type);


private:

  Bmp280 baro;
  GPS gps;
  IMU imu;
  /* Shared Address spaces for M4 and M7 in D3 Region */
  volatile struct acc_data *acc_values_m4 = (struct acc_data*) 0x38001000;
  volatile struct gyro_data *gyro_values_m4 = (struct gyro_data*) 0x38001020;
  volatile struct mag_data *mag_values_m4 = (struct mag_data*) 0x38001040;
  volatile struct baro_data *baro_values_m4 = (struct baro_data*) 0x38001060;
  volatile struct gps_data *gps_values_m4 = (struct gps_data*) 0x38001080;


  volatile uint32_t notif_rx;
  uint32_t gps_timer;
  uint32_t accel_timer;
  uint32_t gyro_timer;
  uint32_t mag_timer;
  uint32_t baro_timer;


};






