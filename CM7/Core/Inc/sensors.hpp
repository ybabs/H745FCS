#pragma once


#include <helpers.hpp>
#include <stdint.h>


//static constexpr uint16_t HEART_BEAT_RATE_MS = 1000;  // 1Hz   //TODO MOVE SOMEWHERE ELSE
//static constexpr uint8_t  GPS_UPDATE_RATE_MS = 100;     // 10 Hz
//static constexpr uint8_t  BARO_UPDATE_RATE_MS = 38;      //~26 Hz
//static constexpr uint8_t  MAG_UPDATE_RATE_MS = 12;      // ~80Hz
//static constexpr uint8_t  GYRO_UPDATE_RATE_MS = 1;       // ~952 Hz
//static constexpr uint8_t  ACC_UPDATE_RATE_MS = 1;       // ~952 Hz

class SensorData
{
public:
	explicit SensorData();
	void ReadRawData();
	void ReadGPS();
	void ReadMag();
	void ReadAcc();
	void ReadBaro();
	void ReadGyro();
	acc_data GetAccData();
	gyro_data GetGyroData();
	mag_data  GetMagData();
	baro_data GetBaroData();
	gps_data GetGpsData();



private:
	  volatile struct acc_data *acc_values_m7 = (struct acc_data*) 0x38001000;
	  volatile struct gyro_data *gyro_values_m7 = (struct gyro_data*) 0x38001020;
	  volatile struct mag_data *mag_values_m7 = (struct mag_data*) 0x38001040;
	  volatile struct baro_data *baro_values_m7 = (struct baro_data*) 0x38001060;
	  volatile struct gps_data *gps_values_m7 = (struct gps_data*) 0x38001080;


	  void HeartBeat();   //TODO MOVE SOMEWHERE ELSE

	  gps_data gps_values;
	  acc_data acc_values;
	  gyro_data gyro_values;
	  baro_data baro_values;
	  mag_data mag_values;

//	  uint8_t led_state;
//	  uint32_t usb_timer = 0;
//	  uint32_t acc_timer = 0;
//	  uint32_t gyro_timer = 0;
//	  uint32_t gps_timer = 0;
//	  uint32_t mag_timer = 0;
//	  uint32_t baro_timer = 0;
//	  uint32_t heartbeat_timer = 0;

};
