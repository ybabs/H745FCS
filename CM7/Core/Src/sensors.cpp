#include <sensors.hpp>
#include "intercore_comms.h"

volatile uint32_t acc_notify;
volatile uint32_t gyro_notify;
volatile uint32_t mag_notify;
volatile uint32_t baro_notify;
volatile uint32_t gps_notify;





SensorData::SensorData()
{

	accel_timer = 0;
	gyro_timer = 0;
	gps_timer = 0;
	mag_timer = 0;
	baro_timer = 0;

}

void SensorData::ReadRawData()
{

	//EnableTiming();
//	  x = *DWT_CYCCNT;
    ReadAcc();
    ReadGyro();
    ReadMag();
    ReadGPS();
    ReadBaro();
//    y = *DWT_CYCCNT;
//    Cycles = (y - x);


}

void SensorData::ReadFilteredData()
{
	// Add filtered data here....
}

void SensorData::ReadGPS()
{

	if((HAL_GetTick() - gps_timer) >= GPS_UPDATE_RATE_MS)
	{
		gps_notify = 0;
		HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_GPS));
		while(gps_notify == 0){}
		gps_values.gps_latitude = gps_values_m7->gps_latitude;
		gps_values.gps_longitude = gps_values_m7->gps_longitude;
		gps_values.gps_altitude = gps_values_m7->gps_altitude;
		gps_values.gps_velocity_x = gps_values_m7->gps_velocity_x;
		gps_values.gps_velocity_y = gps_values_m7->gps_velocity_y;
		gps_values.gps_velocity_z = gps_values_m7->gps_velocity_z;
		gps_values.gps_satellites = gps_values_m7->gps_satellites;
		gps_values.iTOW = gps_values_m7->iTOW;
		gps_values.magDec = gps_values_m7->magDec;
	}

}


void SensorData::ReadMag()
{
	if((HAL_GetTick() - mag_timer) >= MAG_UPDATE_RATE_MS)
	{
		mag_notify = 0;
		HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_MAG));
		while(mag_notify == 0){}
		mag_values.x = mag_values_m7->x;
		mag_values.y = mag_values_m7->y;
		mag_values.z = mag_values_m7->z;
		mag_timer = HAL_GetTick();
	}
}

void SensorData::ReadAcc()
{
	if((HAL_GetTick() - accel_timer) >= ACC_UPDATE_RATE_MS)
	{
		acc_notify = 0;
		HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ACC));
		while(acc_notify == 0){}
		acc_values.x = acc_values_m7->x;
		acc_values.y = acc_values_m7->y;
		acc_values.z = acc_values_m7->z;
		accel_timer = HAL_GetTick();
	}
}

void SensorData::ReadGyro()
{
	if((HAL_GetTick() - gyro_timer) >= GYRO_UPDATE_RATE_MS)
	{
		gyro_notify = 0;
		HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_GYRO));
		while(gyro_notify == 0){}
		gyro_values.x = gyro_values_m7->x;
		gyro_values.y = gyro_values_m7->y;
		gyro_values.z = gyro_values_m7->z;
		gyro_timer = HAL_GetTick();
	}
}


void SensorData::ReadBaro()
{
	if((HAL_GetTick() - baro_timer) >= BARO_UPDATE_RATE_MS)
	{
		baro_notify = 0;
		HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_BARO));
		while(baro_notify == 0){}
		baro_values.baro_altitude = baro_values_m7->baro_altitude;
		baro_values.baro_pressure = baro_values_m7->baro_pressure;
		baro_timer = HAL_GetTick();
	}
}



accelData SensorData::GetAccData()
{
	return acc_values;
}
gyroData SensorData::GetGyroData()
{
	return gyro_values;
}
magData  SensorData::GetMagData()
{
	return mag_values;
}
baroData SensorData::GetBaroData()
{
	return baro_values;
}
gpsData SensorData::GetGpsData()
{
	return gps_values;
}

/**
  * @brief Semaphore Released Callback.
  * @param SemMask: Mask of Released semaphores
  * @retval None
  */
void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
	if(SemMask == SEM_MASK_ACC)
	{
		acc_notify = 1;
	}
	if(SemMask == SEM_MASK_GYRO)
	{
		gyro_notify = 1;
	}
	if(SemMask == SEM_MASK_MAG)
	{
		mag_notify = 1;
	}
	if(SemMask == SEM_MASK_BARO)
	{
		baro_notify = 1;
	}
	if(SemMask == SEM_MASK_GPS)
	{
		gps_notify = 1;
	}
}

