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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// Update Rates of sensors in milliseconds
const uint32_t GPS_SAMPLE_TIME =  100; // GPS has 10HZ update rate
const uint32_t MAG_SAMPLE_TIME =  13; // 80 Hz
const uint32_t IMU_SAMPLE_TIME =  1; // 952HZ
const uint32_t BARO_SAMPLE_TIME = 38; // 26.3 Hz


uint32_t gps_timer = 0;
uint32_t accel_timer = 0;
uint32_t gyro_timer = 0;
uint32_t mag_timer = 0;
uint32_t baro_timer = 0;
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
LSM9DS1Handle imu;
GPSHandle gps;
BMP280Handle baro;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
void GpsTask(void);
void AccelTask(void);
void GyroTask(void);
void MagTask(void);
void BaroTask(void);
void ReadSensors(void);
void ConfigSensors(void);
void M4DataToM7(void);
/* USER CODE END PFP */

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
  MPU_Config();

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
void GpsTask(void)
{
  // Setup an update of the GPS sensor accroding to the update rate
  //if(HAL_GetTick() - gps_timer >= GPS_SAMPLE_TIME)
 // {
     processGPS(&gps);

  //   gps_timer += GPS_SAMPLE_TIME;
 // }

}
void AccelTask(void)
{
 // if(HAL_GetTick() - accel_timer >= IMU_SAMPLE_TIME)
  //{
    readAccel(&imu);

    //accel_timer+=IMU_SAMPLE_TIME;

  //}

}
void GyroTask(void)
{
  //if(HAL_GetTick() - gyro_timer >= IMU_SAMPLE_TIME)
//  {
    readGyro(&imu);

 //   gyro_timer+=IMU_SAMPLE_TIME;

//  }

}
void MagTask(void)
{

 // if(HAL_GetTick() - mag_timer >= MAG_SAMPLE_TIME)
 // {
    readMag(&imu);

   // mag_timer+=MAG_SAMPLE_TIME;

 // }

}

void BaroTask(void)
{
  //if(HAL_GetTick() - baro_timer >= BARO_SAMPLE_TIME)
  //{

    ReadTemp(&baro);
    ReadPressure(&baro);
    ReadAltitude(&baro);

    //baro_timer+=BARO_SAMPLE_TIME;

 // }


}
void ReadSensors(void)
{
  GpsTask();

  AccelTask();

  GyroTask();

  MagTask();

  BaroTask();

}
void M4DataToM7(void)
{

}





void ConfigSensors(void)
{

  // COnfigure GPS Sensor
  ConfigGPS();

  // Configure BMP280
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
  uint8_t baro_res = CheckBMP280ChipID();
  if(baro_res == HAL_OK)
  {
    uint8_t reset_chip_ok = ResetBMP280();
    HAL_Delay(1000);
    uint8_t set_config_ok = setConfig(&baro);
    ReadCalibCoefficients(&baro);
  }

  HAL_Delay(1000);

  // Configure IMU;
  uint16_t imu_res = setup(&imu);


}
/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x10000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

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
