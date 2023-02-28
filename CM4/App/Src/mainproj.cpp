#include "mainproj.hpp"
#include "helpers.hpp"
#include "intercore_comms.h"



//uint32_t x, y;
//uint32_t Cycles;
volatile int notify_cm7 = 0;
//volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)0xE0001004;
//volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000;
//volatile unsigned int *DWT_LAR      = (volatile unsigned int *)0xE0001FB0;
//volatile unsigned int *SCB_DHCSR    = (volatile unsigned int *)0xE000EDF0;
//volatile unsigned int *SCB_DEMCR    = (volatile unsigned int *)0xE000EDFC;
//volatile unsigned int *ITM_TER      = (volatile unsigned int *)0xE0000E00;
//volatile unsigned int *ITM_TCR      = (volatile unsigned int *)0xE0000E80;
//
//static int Debug_ITMDebug = 0;

//inline void EnableTiming(void)
//{
//  if ((*SCB_DHCSR & 1) && (*ITM_TER & 1)) // Enabled?
//    Debug_ITMDebug = 1;
//
//  *SCB_DEMCR |= 0x01000000;
//  *DWT_LAR = 0xC5ACCE55; // enable access
//  *DWT_CYCCNT = 0; // reset the counter
//  *DWT_CONTROL |= 1 ; // enable the counter
//}

EntryApp::EntryApp()
{

}

/*
 * Runs all sensors and
 * processes sensor data.
 * @note, You can comment out a task if you don't want
 * data from it.
 * @retval None.
 */
void EntryApp::RunSensors()
{
   BaroTask();

   GPSTask();

   MagTask();

   AccelTask();

   GyroTask();


}


/*
 * @brief Processes the Accelerometer
 * data from the IMU
 * @retval none
 */
void EntryApp::AccelTask()
{
	HAL_HSEM_FastTake(HSEM_ACC);
//	EnableTiming();
//	x = *DWT_CYCCNT;
	accelData accel = imu.GetAccelData();
//	y = *DWT_CYCCNT;
//	Cycles = (y - x);
	acc_values_m4->x = accel.x;
	acc_values_m4->y = accel.y;
	acc_values_m4->z = accel.z;
	//HAL_HSEM_FastTake(HSEM_ACC);
	HAL_HSEM_Release(HSEM_ACC, 0);
}

/*
 * @brief Processes Mag Data from IMU
 * @retval None
 */
void EntryApp::MagTask()
{
	HAL_HSEM_FastTake(HSEM_MAG);
	magData mag = imu.GetMagData();
	mag_values_m4->x = mag.x;
	mag_values_m4->y = mag.y;
	mag_values_m4->z = mag.z;
	//HAL_HSEM_FastTake(HSEM_MAG);
	HAL_HSEM_Release(HSEM_MAG, 0);
}

/*
 * @brief Processes Gyro data
 * from the IMU
 * @retval none
 */
void EntryApp::GyroTask()
{
	HAL_HSEM_FastTake(HSEM_GYRO);
	gyroData gyro = imu.GetGyroData();
	gyro_values_m4->x = gyro.x;
	gyro_values_m4->y = gyro.y;
	gyro_values_m4->z = gyro.z;
//	HAL_HSEM_FastTake(HSEM_GYRO);
	HAL_HSEM_Release(HSEM_GYRO, 0);

}

/*
 * @brief Processes barometer data from the
 * BMP280
 * @retval None
 */
void EntryApp::BaroTask()
{
	HAL_HSEM_FastTake(HSEM_BARO);
    baro.Run();
    baro_values_m4->baro_altitude = baro.GetAltitude();
    baro_values_m4->baro_pressure = baro.GetPressure();
//	HAL_HSEM_FastTake(HSEM_BARO);
	HAL_HSEM_Release(HSEM_BARO, 0);
}

/*
 * @brief Processes the GPS data
 * @retval none
 */
void EntryApp::GPSTask()
{
   HAL_HSEM_FastTake(HSEM_GPS);
   gps.ProcessGPS();
   gpsData gpsData = gps.GetGPSData();
   gps_values_m4->gps_latitude = gpsData.gps_latitude;
   gps_values_m4->gps_longitude = gpsData.gps_longitude;
   gps_values_m4->gps_altitude = gpsData.gps_altitude;
   gps_values_m4->gps_velocity_x = gpsData.gps_velocity_x;
   gps_values_m4->gps_velocity_y = gpsData.gps_velocity_y;
   gps_values_m4->gps_velocity_z = gpsData.gps_velocity_z;
   gps_values_m4->gps_satellites = gpsData.gps_satellites;
   gps_values_m4->iTOW = gpsData.iTOW;
   gps_values_m4->magDec = gpsData.magDec;
   HAL_HSEM_Release(HSEM_GPS, 0);
}


/**
  * @brief Semaphore Released Callback.
  * @param SemMask: Mask of Released semaphores
  * @retval None
  */
void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
  //notify_cm7 |= SemMask;
}

