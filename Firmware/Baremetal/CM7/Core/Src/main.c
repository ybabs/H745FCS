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
#include "micros.h"

#define USE_FILTER 1


#define ACC_LPF_WEIGHT  0.09f
#define GYRO_LPF_WEIGHT 0.05f
#define MAG_LPF_WEIGHT  1.0f


#define USE_MADGWICK  1
#define USE_KALMAN        0

#define USB_UPDATE_RATE_MS  10 // 100 Hz Update Rate
#define HEART_BEAT_RATE_MS 1000 // 1 Hz

#define G_ACCEL  0.981f   // Acceleration due to gravity value for error bias correction


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
void HeartBeat(void);
void Tick(void);

void ReadRawData(void);
void ReadFilteredData(void);
void GetIMUBias(void);


volatile struct acc_data *acc_values_m7 = (struct acc_data*) 0x38001000;
volatile struct gyro_data *gyro_values_m7 = (struct gyro_data*) 0x3800100D;
volatile struct mag_data *mag_values_m7 = (struct mag_data*) 0x3800101A;
volatile struct baro_data *baro_values_m7 = (struct baro_data*) 0x38001028;
volatile struct gps_data *gps_values_m7 = (struct gps_data*) 0x38001032;
uint32_t usb_timer = 0;
uint32_t heartbeat_timer = 0;
uint8_t led_state = 0x0;

uint8_t sbus_buffer[SBUS_PACKET_LEN];

madgwick_filter_t filter;

RCFilter testImu[3];

//float ax_x;

uint32_t prev;
volatile struct gps_data gps_values;
volatile struct acc_data acc_values;
volatile struct gyro_data gyro_values;
volatile struct baro_data baro_values;
volatile struct mag_data mag_values;


volatile struct acc_data filtered_acc_values;
volatile struct gyro_data filtered_gyro_values;
volatile struct mag_data filtered_mag_values;


volatile struct acc_data  prev_acc;
volatile struct gyro_data prev_gyro;
volatile struct mag_data  prev_mag;

volatile struct mag_data mag_error;
volatile struct acc_data acc_error;
volatile struct gyro_data gyro_error;

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
//static void UART4_Start(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  DWT_Init();

  __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart4, sbus_buffer, SBUS_PACKET_LEN);


  GetIMUBias();

   char errBuf[256];

   sprintf(errBuf, "IMU Bias: %.4f, %.4f, %.4f\r\n", acc_error.imu_acc_x, acc_error.imu_acc_y, acc_error.imu_acc_z);

  //MadgwickInit();
  while (1)
  {
    //Tick();

    #ifdef USE_FILTER
     ReadFilteredData();
     //ReadRawData();
    #else
      ReadRawData();
    #endif
    //filter.dt = 1.0f/5000.0f;
    //MadgwickQuaternionUpdate(&filter, &acc_values, &gyro_values, &mag_values);
    if((HAL_GetTick() - usb_timer) >= USB_UPDATE_RATE_MS)
    {
      char logBuf[256];
      //sprintf(logBuf, "%.4f, %.4f, %.4f\r\n", filter.roll, filter.pitch, filter.yaw);

      #ifdef USE_FILTER
        sprintf(logBuf, "ax:%.4f, ay:%.4f, az:%.4f, gx:%.4f, gy:%.4f, gz:%.4f, mx:%.4f, my:%.4f, mz:%.4f\r\n",
              filtered_acc_values.imu_acc_x, filtered_acc_values.imu_acc_y, filtered_acc_values.imu_acc_z, filtered_gyro_values.imu_gyro_x,
              filtered_gyro_values.imu_gyro_y, filtered_gyro_values.imu_gyro_z, filtered_mag_values.imu_mag_x, filtered_mag_values.imu_mag_y, filtered_mag_values.imu_mag_z);
      #else
        sprintf(logBuf, "ax:%.4f, ay:%.4f, az:%.4f, gx:%.4f, gy:%.4f, gz:%.4f, mx:%.4f, my:%.4f, mz:%.4f\r\n",
                      acc_values.imu_acc_x, acc_values.imu_acc_y, acc_values.imu_acc_z, gyro_values.imu_gyro_x, gyro_values.imu_gyro_y, gyro_values.imu_gyro_z,
                      mag_values.imu_mag_x, mag_values.imu_mag_y, mag_values.imu_mag_z);
      #endif

//      CDC_Transmit_FS((uint8_t *) errBuf, strlen(errBuf));
     CDC_Transmit_FS((uint8_t *) logBuf, strlen(logBuf));
      usb_timer = HAL_GetTick();
    }
   HeartBeat();
  }
}

void HeartBeat(void)
{
  if((HAL_GetTick() - heartbeat_timer) >= HEART_BEAT_RATE_MS)
     {
        if(led_state == 0)
        {
          LEDA_OFF();
          led_state = 1;
        }

        else if(led_state == 1)
        {
          LEDA_ON();
          led_state = 0;
        }

        heartbeat_timer = HAL_GetTick();
     }
}

void ReadRawData(void)
{
    ReadGPS();
    ReadMag();
    ReadAcc();
    ReadBaro();
    ReadGyro();
}

/*
 * @brief Returns a low passed Filtered IMU data
 */
void ReadFilteredData(void)
{

  // Get Raw IMU Data

  ReadAcc();
  // Correct Output with bias
  filtered_acc_values.imu_acc_x = acc_values.imu_acc_x - acc_error.imu_acc_x;
  filtered_acc_values.imu_acc_y = acc_values.imu_acc_y - acc_error.imu_acc_y;
  filtered_acc_values.imu_acc_z = acc_values.imu_acc_z - acc_error.imu_acc_z;

  filtered_acc_values.imu_acc_x = (1.0f - ACC_LPF_WEIGHT)* prev_acc.imu_acc_x + ACC_LPF_WEIGHT * filtered_acc_values.imu_acc_x;
  filtered_acc_values.imu_acc_y = (1.0f - ACC_LPF_WEIGHT)* prev_acc.imu_acc_y + ACC_LPF_WEIGHT * filtered_acc_values.imu_acc_y;
  filtered_acc_values.imu_acc_z = (1.0f - ACC_LPF_WEIGHT)* prev_acc.imu_acc_z + ACC_LPF_WEIGHT * filtered_acc_values.imu_acc_z;

  prev_acc = filtered_acc_values;

  // GYro
  ReadGyro();
  // Correct Output with bias
  filtered_gyro_values.imu_gyro_x = gyro_values.imu_gyro_x - gyro_error.imu_gyro_x;
  filtered_gyro_values.imu_gyro_y = gyro_values.imu_gyro_y - gyro_error.imu_gyro_y;
  filtered_gyro_values.imu_gyro_z = gyro_values.imu_gyro_z - gyro_error.imu_gyro_z;

  filtered_gyro_values.imu_gyro_x = (1.0f - GYRO_LPF_WEIGHT ) * prev_gyro.imu_gyro_x + GYRO_LPF_WEIGHT * filtered_gyro_values.imu_gyro_x;
  filtered_gyro_values.imu_gyro_y = (1.0f - GYRO_LPF_WEIGHT ) * prev_gyro.imu_gyro_y + GYRO_LPF_WEIGHT * filtered_gyro_values.imu_gyro_y;
  filtered_gyro_values.imu_gyro_z = (1.0f - GYRO_LPF_WEIGHT ) * prev_gyro.imu_gyro_z + GYRO_LPF_WEIGHT * filtered_gyro_values.imu_gyro_z;

  prev_gyro = filtered_gyro_values;

 // MAG
  ReadMag();
  filtered_mag_values.imu_mag_x = mag_values.imu_mag_x - mag_error.imu_mag_x;
  filtered_mag_values.imu_mag_y = mag_values.imu_mag_y - mag_error.imu_mag_y;
  filtered_mag_values.imu_mag_z = mag_values.imu_mag_z - mag_error.imu_mag_z;

  // Don't filter mag values
  filtered_mag_values.imu_mag_x = (1.0f - MAG_LPF_WEIGHT) * prev_mag.imu_mag_x + MAG_LPF_WEIGHT * filtered_mag_values.imu_mag_x;
  filtered_mag_values.imu_mag_y = (1.0f - MAG_LPF_WEIGHT) * prev_mag.imu_mag_y + MAG_LPF_WEIGHT * filtered_mag_values.imu_mag_y;
  filtered_mag_values.imu_mag_z = (1.0f - MAG_LPF_WEIGHT) * prev_mag.imu_mag_z + MAG_LPF_WEIGHT * filtered_mag_values.imu_mag_z;

  prev_mag = filtered_mag_values;

}

/*
 * Gets a bias term of IMU when FC is at rest.
 * Crappy way of removing bias term
 */
void GetIMUBias(void)
{
  int loop_term = 0;

  acc_error.imu_acc_x = 0.0f;
  acc_error.imu_acc_y = 0.0f;
  acc_error.imu_acc_z = 0.0f;

  mag_error.imu_mag_x = 0.0f;
  mag_error.imu_mag_y = 0.0f;
  mag_error.imu_mag_z = 0.0f;

  gyro_error.imu_gyro_x = 0.0f;
  gyro_error.imu_gyro_y = 0.0f;
  gyro_error.imu_gyro_z = 0.0f;

  HAL_Delay(10000);


  while(loop_term < 10000)
  {
    ReadAcc();

    acc_error.imu_acc_x += acc_values.imu_acc_x;
    acc_error.imu_acc_y += acc_values.imu_acc_y;
    acc_error.imu_acc_z += acc_values.imu_acc_z;

    ReadGyro();

    gyro_error.imu_gyro_x += gyro_values.imu_gyro_x;
    gyro_error.imu_gyro_y += gyro_values.imu_gyro_y;
    gyro_error.imu_gyro_z += gyro_values.imu_gyro_z;

    ReadMag();
    mag_error.imu_mag_x += mag_values.imu_mag_x;
    mag_error.imu_mag_y += mag_values.imu_mag_y;
    mag_error.imu_mag_z += mag_values.imu_mag_z;

    loop_term++;
  }

  // Get error value
  acc_error.imu_acc_x /= loop_term;
  acc_error.imu_acc_y /= loop_term;
  acc_error.imu_acc_z /= loop_term;
  acc_error.imu_acc_z -= G_ACCEL;

  mag_error.imu_mag_x /= loop_term;
  mag_error.imu_mag_y /= loop_term;
  mag_error.imu_mag_z /= loop_term;

  gyro_error.imu_gyro_x /= loop_term;
  gyro_error.imu_gyro_y /= loop_term;
  gyro_error.imu_gyro_z /= loop_term;
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

void Tick(void)
{
  uint32_t now = micros();
  filter.dt = (now - prev) / 1000000.0f;
  prev = now;
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
