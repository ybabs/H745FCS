#include "mainproj.hpp"
#include "helpers.hpp"

static volatile uint32_t notif_rx;


EntryApp::EntryApp()
{

}

/*
 * @brief Processes barometer data from the
 * BMP280
 * @retval None
 */
void EntryApp::BaroTask()
{
    baro.Run();
    M4DataToM7(BARO_DATA_TYPE);
}

/*
 * @brief Processes the GPS data
 * @retval none
 */
void EntryApp::GPSTask()
{
   gps.ProcessGPS();
   M4DataToM7(GPS_DATA_TYPE);
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

   AccelTask();

   MagTask();

   GyroTask();
}


/*
 * @brief Processes the Accelerometer
 * data from the IMU
 * @retval none
 */
void EntryApp::AccelTask()
{
  imu.GetAccelData();
  M4DataToM7(ACC_DATA_TYPE);
}

/*
 * @brief Processes Mag Data from IMU
 * @retval None
 */
void EntryApp::MagTask()
{

  imu.GetMagData();
  M4DataToM7(MAG_DATA_TYPE);

}

/*
 * @brief Processes Gyro data
 * from the IMU
 * @retval none
 */
void EntryApp::GyroTask()
{

  imu.GetGyroData();
  M4DataToM7(GYRO_DATA_TYPE);

}

/*
 * @brief Uses the Hardware Semaphore to send sensor data
 * from the M4 core to the M7.
 * @param data_type Sensor data type to send
 * @retval None
 */
void EntryApp::M4DataToM7(const uint8_t data_type)
{
	switch(data_type)
	{
		case GPS_DATA_TYPE:
		{

			if (HAL_HSEM_FastTake(HSEM_ID_1) == HAL_OK)
			    {

			      gps_values_m4->gps_latitude = gps.GetGPSData().gps_latitude;
			      gps_values_m4->gps_longitude = gps.GetGPSData().gps_longitude;
			      gps_values_m4->gps_altitude = gps.GetGPSData().gps_altitude;
			      gps_values_m4->gps_velocity_x = gps.GetGPSData().gps_velocity_x;
			      gps_values_m4->gps_velocity_y = gps.GetGPSData().gps_velocity_y;
			      gps_values_m4->gps_velocity_z = gps.GetGPSData().gps_velocity_z;
			      gps_values_m4->gnd_speed = gps.GetGPSData().gnd_speed;
			      gps_values_m4->gps_satellites = gps.GetGPSData().gps_satellites;
			    }
			    // Release semaphore
			    HAL_HSEM_Release(HSEM_ID_1, 0);
			break;
		}

		case ACC_DATA_TYPE:
		{
		    if (HAL_HSEM_FastTake(HSEM_ID_1) == HAL_OK)
		    {
		      acc_values_m4->imu_acc_x = imu.GetAccelData().x;
		      acc_values_m4->imu_acc_y = imu.GetAccelData().y;
		      acc_values_m4->imu_acc_z = imu.GetAccelData().z;
		    }
		    // Release semaphore
		    HAL_HSEM_Release(HSEM_ID_1, 0);
			break;
		}

		case GYRO_DATA_TYPE:
		{
		    if (HAL_HSEM_FastTake(HSEM_ID_1) == HAL_OK)
		    {
		      gyro_values_m4->imu_gyro_x = imu.GetGyroData().x;
		      gyro_values_m4->imu_gyro_y = imu.GetGyroData().y;
		      gyro_values_m4->imu_gyro_z = imu.GetGyroData().z;
		    }
		    // Release semaphore
		    HAL_HSEM_Release(HSEM_ID_1, 0);

			break;
		}

		case MAG_DATA_TYPE:
		{
		    if (HAL_HSEM_FastTake(HSEM_ID_1) == HAL_OK)
		    {
		      mag_values_m4->imu_mag_x = imu.GetMagData().x;
		      mag_values_m4->imu_mag_y = imu.GetMagData().y;
		      mag_values_m4->imu_mag_z = imu.GetMagData().z;
		    }
		    // Release semaphore
		    HAL_HSEM_Release(HSEM_ID_1, 0);
			break;
		}

		case BARO_DATA_TYPE:
		{
		    if (HAL_HSEM_FastTake(HSEM_ID_1) == HAL_OK)
		    {
		      baro_values_m4->baro_altitude = baro.GetAltitude();
		      baro_values_m4->baro_pressure = baro.GetPressure();
		    }
		    // Release semaphore
		    HAL_HSEM_Release(HSEM_ID_1, 0);
			break;
		}
	}
}


/**
  * @brief Semaphore Released Callback.
  * @param SemMask: Mask of Released semaphores
  * @retval None
  */
void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
  notif_rx = 1;
}

