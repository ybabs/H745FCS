#pragma once


#include <helpers.hpp>
#include <stdint.h>


class SensorData
{
public:
	explicit SensorData();
	void ReadRawData();
	void ReadFilteredData();
	void ReadGPS();
	void ReadMag();
	void ReadAcc();
	void ReadBaro();
	void ReadGyro();
	accelData GetAccData();
	gyroData GetGyroData();
	magData  GetMagData();
	baroData GetBaroData();
	gpsData GetGpsData();



private:
	  volatile struct accelData *acc_values_m7 = (struct accelData*) ACCEL_D3_ADDRESS;
	  volatile struct gyroData *gyro_values_m7 = (struct gyroData*) GYRO_D3_ADDRESS;
	  volatile struct magData *mag_values_m7 = (struct magData*) MAG_D3_ADDRESS;
	  volatile struct baroData *baro_values_m7 = (struct baroData*) BARO_D3_ADDRESS;
	  volatile struct gpsData *gps_values_m7 = (struct gpsData*) GPS_D3_ADDRESS;


	  void HeartBeat();   //TODO MOVE SOMEWHERE ELSE

	  gpsData gps_values;
	  accelData acc_values;
	  gyroData gyro_values;
	  baroData baro_values;
	  magData mag_values;

	  uint32_t accel_timer;
	  uint32_t gyro_timer;
	  uint32_t mag_timer;
	  uint32_t gps_timer;
	  uint32_t baro_timer;





};
