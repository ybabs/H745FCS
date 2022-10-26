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


};
