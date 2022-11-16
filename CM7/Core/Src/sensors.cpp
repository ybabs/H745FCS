#include <sensors.hpp>


SensorData::SensorData()
{

}

void SensorData::ReadRawData()
{
//    ReadGPS();
//    ReadMag();
//    ReadAcc();
//    ReadBaro();
    ReadGyro();
}

void SensorData::ReadFilteredData()
{
	// Add filtered data here....
}

void SensorData::ReadGPS()
{
	  if(HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
	      {
	        gps_values.gps_latitude = gps_values_m7->gps_latitude;
	        gps_values.gps_longitude = gps_values_m7->gps_longitude;
	        gps_values.gps_altitude = gps_values_m7->gps_altitude;
	        gps_values.gps_velocity_x = gps_values_m7->gps_velocity_x;
	        gps_values.gps_velocity_y = gps_values_m7->gps_velocity_y;
	        gps_values.gps_velocity_z = gps_values_m7->gps_velocity_z;
	        gps_values.gps_satellites = gps_values_m7->gps_satellites;
	      }
	      HAL_HSEM_Release(HSEM_ID_0,0);
}


void SensorData::ReadMag()
{
	  if(HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
	  {
	    mag_values.imu_mag_x = mag_values_m7->imu_mag_x;
	    mag_values.imu_mag_y = mag_values_m7->imu_mag_y;
	    mag_values.imu_mag_z = mag_values_m7->imu_mag_z;
	  }
	  HAL_HSEM_Release(HSEM_ID_0,0);

}

void SensorData::ReadAcc()
{
	if(HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
	  {
	      acc_values.imu_acc_x = acc_values_m7->imu_acc_x;
	      acc_values.imu_acc_y = acc_values_m7->imu_acc_y;
	      acc_values.imu_acc_z = acc_values_m7->imu_acc_z;
	  }
	  HAL_HSEM_Release(HSEM_ID_0,0);
}

void SensorData::ReadBaro()
{
	  if(HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
	  {
	     baro_values.baro_altitude = baro_values_m7->baro_altitude;
	     baro_values.baro_pressure = baro_values_m7->baro_pressure;
	  }
	  HAL_HSEM_Release(HSEM_ID_0,0);
}

void SensorData::ReadGyro()
{
	  if(HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
	  {
	     gyro_values.imu_gyro_x = gyro_values_m7->imu_gyro_x;
	     gyro_values.imu_gyro_y = gyro_values_m7->imu_gyro_y;
	     gyro_values.imu_gyro_z = gyro_values_m7->imu_gyro_z;
	  }
	  HAL_HSEM_Release(HSEM_ID_0,0);
}

acc_data SensorData::GetAccData()
{
	return acc_values;
}
gyro_data SensorData::GetGyroData()
{
	return gyro_values;
}
mag_data  SensorData::GetMagData()
{
	return mag_values;
}
baro_data SensorData::GetBaroData()
{
	return baro_values;
}
gps_data SensorData::GetGpsData()
{
	return gps_values;
}
