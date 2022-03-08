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
#include "fatfs.h"
#include "i2c.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"


#define USE_MADGWICK  1
#define USE_COMPLEMENTARY 0
#define USE_KALMAN        0

#define USB_UPDATE_RATE_MS  10 // 100 Hz Update Rate

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "fir.h"
#include "rc.h"

#include "sbus.h"
#include "madgwick.h"

void ReadGPS(void);
void ReadMag(void);
void ReadAcc(void);
void ReadBaro(void);
void ReadGyro(void);

void ComputeEulerAngles(void);

volatile struct acc_data *acc_values_m7 = (struct acc_data*) 0x38001000;
volatile struct gyro_data *gyro_values_m7 = (struct gyro_data*) 0x3800100D;
volatile struct mag_data *mag_values_m7 = (struct mag_data*) 0x3800101A;
volatile struct baro_data *baro_values_m7 = (struct baro_data*) 0x38001028;
volatile struct gps_data *gps_values_m7 = (struct gps_data*) 0x38001032;
uint32_t usb_timer = 0;


uint8_t sbus_buffer[SBUS_PACKET_LEN];

/* to calculate elapsed time for integration */
float dt = 0.0f;
float roll = 0.0f;
float pitch = 0.0f;
float yaw  = 0.0f;
//uint32_t tick_now = 0;
//uint32_t tick_prev = 0;
uint32_t count = 0;
uint32_t sum_count = 0;

/* USER CODE END Includes */
uint32_t duration_us = 0x00;
uint32_t nb_cycles  = 0x00;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

struct gps_data gps_values;
struct acc_data acc_values;
struct gyro_data gyro_values;
struct baro_data baro_values;
struct mag_data mag_values;

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
void SystemClock_Config(void);
static void MPU_Config(void);
//static void UART4_Start(void);
/* USER CODE BEGIN PFP */


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
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
  int32_t timeout;
/* USER CODE END Boot_Mode_Sequence_0 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;

  while (__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET)
  {
    asm("NOP");
  }
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
HSEM notification */
/*HW semaphore Clock enable*/
__HAL_RCC_HSEM_CLK_ENABLE();
/*Take HSEM */
HAL_HSEM_FastTake(HSEM_ID_0);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_ID_0,0);
/* wait until CPU2 wakes up from stop mode */
timeout = 0xFFFF;
while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
if ( timeout < 0 )
{
Error_Handler();
}
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_UART4_Init();
  MX_I2C2_Init();
 // MX_SDMMC1_SD_Init();
  MX_TIM1_Init();
  MX_FATFS_Init();
  MX_UART7_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart4, sbus_buffer, SBUS_PACKET_LEN);

  /* USER CODE END 2 */
// char txBuf[8];
// uint8_t count = 1;
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
//    ReadGPS();
    ReadMag();
    ReadAcc();
    ReadBaro();
    ReadGyro();

    TimerCount_Start();
    dt = (duration_us)/1000000.0f;
    MadgwickQuaternionUpdate(&acc_values, &gyro_values, &mag_values, dt);
    ComputeEulerAngles();
    TimerCount_Stop(nb_cycles);
    duration_us  = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);

    if((HAL_GetTick() - usb_timer) >= USB_UPDATE_RATE_MS)
    {
      char logBuf[128];
      sprintf(logBuf, "%.4f, %.4f, %.4f\r\n", roll, pitch, yaw);
      CDC_Transmit_FS((uint8_t *) logBuf, strlen(logBuf));
      usb_timer = HAL_GetTick();
    }
  }
  /* USER CODE END 3 */
}


void ReadGPS(void)
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

void ReadMag(void)
{

  if(HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
  {
    mag_values.imu_mag_x = mag_values_m7->imu_mag_x;
    mag_values.imu_mag_y = mag_values_m7->imu_mag_y;
    mag_values.imu_mag_z = mag_values_m7->imu_mag_z;
  }
  HAL_HSEM_Release(HSEM_ID_0,0);

}
void ReadAcc(void)
{
  if(HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
  {
      acc_values.imu_acc_x = acc_values_m7->imu_acc_x;
      acc_values.imu_acc_y = acc_values_m7->imu_acc_y;
      acc_values.imu_acc_z = acc_values_m7->imu_acc_z;
  }
  HAL_HSEM_Release(HSEM_ID_0,0);
}
void ReadBaro(void)
{
  if(HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
  {
     baro_values.baro_altitude = baro_values_m7->baro_altitude;
     baro_values.baro_pressure = baro_values_m7->baro_pressure;
  }
  HAL_HSEM_Release(HSEM_ID_0,0);
}
void ReadGyro(void)
{
  if(HAL_HSEM_FastTake(HSEM_ID_0) == HAL_OK)
  {
     gyro_values.imu_gyro_x = gyro_values_m7->imu_gyro_x;
     gyro_values.imu_gyro_y = gyro_values_m7->imu_gyro_y;
     gyro_values.imu_gyro_z = gyro_values_m7->imu_gyro_z;
  }
  HAL_HSEM_Release(HSEM_ID_0,0);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 15;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable USB Voltage detector
  */
  HAL_PWREx_EnableUSBVoltageDetector();
}

/* USER CODE BEGIN 4 */

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
    MPU_InitStruct.BaseAddress = 0x24000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
    MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.BaseAddress = 0x38000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;
    MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    /* Enables the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void ComputeEulerAngles(void)
{

  // Define output variables from updated quaternion---these are Tait-Bryan
      // angles, commonly used in aircraft orientation. In this coordinate system,
      // the positive z-axis is down toward Earth. Yaw is the angle between Sensor
      // x-axis and Earth magnetic North (or true North if corrected for local
      // declination, looking down on the sensor positive yaw is counterclockwise.
      // Pitch is angle between sensor x-axis and Earth ground plane, toward the
      // Earth is positive, up toward the sky is negative. Roll is angle between
      // sensor y-axis and Earth ground plane, y-axis up is positive roll. These
      // arise from the definition of the homogeneous rotation matrix constructed
      // from quaternions. Tait-Bryan angles as well as Euler angles are
      // non-commutative; that is, the get the correct orientation the rotations
      // must be applied in the correct order which for this configuration is yaw,
      // pitch, and then roll.
      // For more see
      // http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
      // which has additional links.
        yaw   = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ()
                          * *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1)
                          * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) - *(getQ()+3)
                          * *(getQ()+3));
        pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ()
                          * *(getQ()+2)));
        roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2)
                          * *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1)
                          * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) + *(getQ()+3)
                          * *(getQ()+3));

        pitch *= RAD_TO_DEG;
        yaw *= RAD_TO_DEG;
        yaw +=2.43;
        roll *= RAD_TO_DEG;
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == UART4)
  {
    if(sbus_buffer[0] != SBUS_HEADER)
    {
      HAL_UART_DMAStop(&huart4);
      __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    }

    else
    {
      updateSbus(sbus_buffer);
    }
  }

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
