/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ublox.h"
#include "lsm9ds1.h"
#include "bmp280.h"
#include "string.h"
#include "common.h"

/* Shared Address spaces for M4 and M7 in D3 Region */
volatile struct acc_data *acc_values_m4 = (struct acc_data*) 0x38001000;
volatile struct gyro_data *gyro_values_m4 = (struct gyro_data*) 0x3800100D;
volatile struct mag_data *mag_values_m4 = (struct mag_data*) 0x3800101A;
volatile struct baro_data *baro_values_m4 = (struct baro_data*) 0x38001028;
volatile struct gps_data *gps_values_m4 = (struct gps_data*) 0x38001032;



/* Sensor Handles */
LSM9DS1Handle imu;

GPSHandle gps;
BMP280Handle baro;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

uint32_t gps_timer = 0;
uint32_t accel_timer = 0;
uint32_t gyro_timer = 0;
uint32_t mag_timer = 0;
uint32_t baro_timer = 0;

// Variable to send Notification
volatile uint32_t notif_rx;
/* USER CODE END PTD */
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
//static void MPU_Config(void);
/* USER CODE BEGIN PFP */
void GpsTask(void);
void AccelTask(void);
void GyroTask(void);
void MagTask(void);
void BaroTask(void);
void ReadSensors(void);
//void ReadFilteredData(void);
uint8_t ConfigSensors(void);
void M4DataToM7(uint8_t data_type);

//void GetIMUBias(void);
/* USER CODE END PFP */

float temp;
float pressure;
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
 // MPU_Config();

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();
  /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  /*
  Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
  perform system initialization (system clock config, external memory configuration.. )
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);
  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  ConfigSensors();
  /* USER CODE END 2 */

//  GetIMUBias();

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
    ReadSensors();
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
/*
 * @brief Processes the GPS data
 * @retval none
 */
void GpsTask(void)
{
     processGPS(&gps);
  M4DataToM7(GPS_DATA_TYPE);
}

/*
 * @brief Processes the Accelerometer
 * data from the IMU
 * @retval none
 */
void AccelTask(void)
{
    readAccel(&imu);
  M4DataToM7(ACC_DATA_TYPE);
}

/*
 * @brief Processes Gyro data
 * from the IMU
 * @retval none
 */
void GyroTask(void)
{
    readGyro(&imu);
  M4DataToM7(GYRO_DATA_TYPE);
}

/*
 * @brief Processes Mag Data from IMU
 * @retval None
 */
void MagTask(void)
{
  readMag(&imu);
  M4DataToM7(MAG_DATA_TYPE);
}

/*
 * @brief Processes barometer data from the
 * BMP280
 * @retval None
 */
void BaroTask(void)
{
  ReadTemp(&baro);
  ReadPressure(&baro);
  ReadAltitude(&baro);

   M4DataToM7(BARO_DATA_TYPE);

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

/*
 * @brief Returns a low passed Filtered IMU data
 */
//void ReadFilteredData(void)
//{
//
//  // Get Raw IMU Data
//  AccelTask();
//
//  // Correct Output with bias
//  filtered_imu.accel_values.x = imu.accel_values.x - acc_error->imu_acc_x;
//  filtered_imu.accel_values.y = imu.accel_values.y - acc_error->imu_acc_y;
//  filtered_imu.accel_values.z = imu.accel_values.z - acc_error->imu_acc_z;
//
//}


/*
 * Runs all sensors and
 * processes sensor data.
 * @note, You can comment out a task if you don't want
 * data from it.
 * @retval None.
 */
void ReadSensors(void)
{
  GpsTask();

  AccelTask();

  GyroTask();

  MagTask();

  BaroTask();

}

/*
 * Gets a bias term of IMU when FC is at rest.
 * Crappy way of removing bias term
 */
//void GetIMUBias(void)
//{
//  int loop_term;
//
//  while(loop_term < 10000)
//  {
//
//    AccelTask();
//    GyroTask();
//    MagTask();
//
//    acc_error->imu_acc_x += imu.accel_values.x;
//    acc_error->imu_acc_y += imu.accel_values.y;
//    acc_error->imu_acc_z += imu.accel_values.z;
//
//    mag_error->imu_mag_x += imu.mag_values.x;
//    mag_error->imu_mag_y += imu.mag_values.y;
//    mag_error->imu_mag_z += imu.mag_values.z;
//
//    gyro_error->imu_gyro_x += imu.gyro_values.x;
//    gyro_error->imu_gyro_y += imu.gyro_values.y;
//    gyro_error->imu_gyro_z += imu.gyro_values.z;
//
//    loop_term++;
//  }
//
//  // Get error value
//  acc_error->imu_acc_x /= loop_term;
//  acc_error->imu_acc_y /= loop_term;
//  acc_error->imu_acc_z /= loop_term;
//
//  mag_error->imu_mag_x /= loop_term;
//  mag_error->imu_mag_y /= loop_term;
//  mag_error->imu_mag_z /= loop_term;
//
//  gyro_error->imu_gyro_x /= loop_term;
//  gyro_error->imu_gyro_y /= loop_term;
//  gyro_error->imu_gyro_z /= loop_term;
//}

/*
 * @brief Uses the Hardware Semaphore to send sensor data
 * from the M4 core to the M7.
 * @param data_type Sensor data type to send
 * @retval None
 */
void M4DataToM7(uint8_t data_type)
{

  switch (data_type)
  {
  case GPS_DATA_TYPE:
  {
    if (HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
    {

      gps_values_m4->gps_latitude = gps.latitude;
      gps_values_m4->gps_longitude = gps.longitude;
      gps_values_m4->gps_altitude = gps.altitude;
      gps_values_m4->gps_velocity_x = gps.vel_x;
      gps_values_m4->gps_velocity_y = gps.vel_y;
      gps_values_m4->gps_velocity_z = gps.vel_z;
      gps_values_m4->gnd_speed = gps.gndSpeed;
      gps_values_m4->gps_satellites = gps.num_satellites;
    }
    // Release semaphore
    HAL_HSEM_Release(HSEM_ID_0, 0);
    break;
  }

  case ACC_DATA_TYPE:
  {
    if (HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
    {
      acc_values_m4->imu_acc_x = imu.accel_values.x;
      acc_values_m4->imu_acc_y = imu.accel_values.y;
      acc_values_m4->imu_acc_z = imu.accel_values.z;
    }
    // Release semaphore
    HAL_HSEM_Release(HSEM_ID_0, 0);

    break;
  }

  case GYRO_DATA_TYPE:
  {
    if (HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
    {
      gyro_values_m4->imu_gyro_x = imu.gyro_values.x;
      gyro_values_m4->imu_gyro_y = imu.gyro_values.y;
      gyro_values_m4->imu_gyro_z = imu.gyro_values.z;
    }
    // Release semaphore
    HAL_HSEM_Release(HSEM_ID_0, 0);

    break;
  }

  case MAG_DATA_TYPE:
  {

    if (HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
    {
      mag_values_m4->imu_mag_x = imu.mag_values.x;
      mag_values_m4->imu_mag_y = imu.mag_values.y;
      mag_values_m4->imu_mag_z = imu.mag_values.z;
    }
    // Release semaphore
    HAL_HSEM_Release(HSEM_ID_0, 0);
    break;
  }

  case BARO_DATA_TYPE:
  {
    if (HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
    {
      baro_values_m4->baro_altitude = baro.altitude;
      baro_values_m4->baro_pressure = baro.pressure;
    }
    // Release semaphore
    HAL_HSEM_Release(HSEM_ID_0, 0);
    break;
  }

  default:
    break;

  }
}




/*
 * @brief Initialises the sensors
 * @retval HAL_OK if successful
 */
uint8_t ConfigSensors(void)
{


  // COnfigure GPS Sensor
  ConfigGPS();

  HAL_Delay(100);
  // Configure IMU;
  if (ConfigIMU(&imu) != HAL_OK)
  {
    return HAL_ERROR;
  }

  HAL_Delay(100);

  // Configure BMP280
  if (ConfigBMP280(&baro) != HAL_OK)
  {
    return HAL_ERROR;
  }

 return HAL_OK;


}
/* USER CODE END 4 */

/* MPU Configuration */
//MPU not needed in CM4 Core
//void MPU_Config(void)
//{
//  MPU_Region_InitTypeDef MPU_InitStruct = {0};
//
//  /* Disables the MPU */
//  HAL_MPU_Disable();
//  /** Initializes and configures the Region and the memory to be protected
//  */
//  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
//  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
//  MPU_InitStruct.BaseAddress = 0x10000000;
//  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
//  MPU_InitStruct.SubRegionDisable = 0x0;
//  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
//  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
//  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
//  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
//  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
//
//  HAL_MPU_ConfigRegion(&MPU_InitStruct);
//  /* Enables the MPU */
//  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
//
//}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
