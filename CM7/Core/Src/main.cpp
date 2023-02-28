///* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * @file           : main.c
//  * @brief          : Main program body
//  ******************************************************************************
//  * @attention
//  *
//  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
//  * All rights reserved.</center></h2>
//  *
//  * This software component is licensed by ST under BSD 3-Clause license,
//  * the "License"; You may not use this file except in compliance with the
//  * License. You may obtain a copy of the License at:
//  *                        opensource.org/licenses/BSD-3-Clause
//  *
//  ******************************************************************************
//  */
///* USER CODE END Header */
///* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "usbd_cdc_if.h"
#include <micros.hpp>
#include "usb_command_set.h"

#include "helpers.hpp"
#include "intercore_comms.h"
#include <sensors.hpp>
#include <calibration.hpp>
#include <common.h>
#include <sbus.h>
#include <serializer.h>

#include "Eigen"
#include <array>

void SystemClock_Config(void);
static void MPU_Config(void);

SensorData sensors;

accelData acc;
magData mag;
gyroData gyro;
gpsData gps;
baroData baro;

uint32_t usb_timer = 0;


uint32_t x, y;
uint32_t Cycles;

volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)0xE0001004;
volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000;
volatile unsigned int *DWT_LAR      = (volatile unsigned int *)0xE0001FB0;
volatile unsigned int *SCB_DHCSR    = (volatile unsigned int *)0xE000EDF0;
volatile unsigned int *SCB_DEMCR    = (volatile unsigned int *)0xE000EDFC;
volatile unsigned int *ITM_TER      = (volatile unsigned int *)0xE0000E00;
volatile unsigned int *ITM_TCR      = (volatile unsigned int *)0xE0000E80;

static int Debug_ITMDebug = 0;

inline void EnableTiming(void)
{
  if ((*SCB_DHCSR & 1) && (*ITM_TER & 1)) // Enabled?
    Debug_ITMDebug = 1;

  *SCB_DEMCR |= 0x01000000;
  *DWT_LAR = 0xC5ACCE55; // enable access
  *DWT_CYCCNT = 0; // reset the counter
  *DWT_CONTROL |= 1 ; // enable the counter
}


SbusController frkskyRC;
//
///**
//  * @brief  The application entry point.
//  * @retval int
//  */
int main(void)
{
///* USER CODE BEGIN Boot_Mode_Sequence_0 */
  int32_t timeout;
///* USER CODE END Boot_Mode_Sequence_0 */
//
//  /* MPU Configuration--------------------------------------------------------*/
	MPU_Config();
//
//  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

//
//  /* Enable D-Cache---------------------------------------------------------*/
   SCB_EnableDCache();
//
/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;

  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler();
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
HAL_HSEM_FastTake(HSEM_BOOT);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_BOOT,0);
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
  //MX_FATFS_Init();
  MX_UART7_Init();
  MX_USB_DEVICE_Init();
  DWT_Init();

  __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart4, sbus_buffer, SBUS_PACKET_LEN);

  //__HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ACC));

  HAL_NVIC_SetPriority(HSEM1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(HSEM1_IRQn);
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ACC));
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_MAG));
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_GPS));
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_BARO));
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_GYRO));



  while (!CDC_Class_Init_Ok());
  Serializer serializer;
  HAL_Delay(1000);

  while (1)
  {


	  if((HAL_GetTick() - usb_timer) >= USB_UPDATE_RATE_MS)
	  {

	  	  //Read raw data first
	  	  sensors.ReadRawData();

		  //char logBuf[256];
		  acc = sensors.GetAccData();
		  gyro = sensors.GetGyroData();
		  mag = sensors.GetMagData();
		  gps = sensors.GetGpsData();
		  baro = sensors.GetBaroData();
		  // send data through USB

		//HAL_Delay(1);
		   //EnableTiming();
		   //x = *DWT_CYCCNT;
		   serializer.SendData(acc);
		   delay_us(500);
		   serializer.SendData(mag);
		   delay_us(500);
		   serializer.SendData(gyro);
		   delay_us(500);
		   serializer.SendData(gps);
 		   delay_us(500);
		   serializer.SendData(baro);
		   //delay_us(500);
//		   y = *DWT_CYCCNT;
//		   Cycles = (y - x);

		  // sprintf(logBuf, "lat:%.10f lon:%.10f baro_alt:%.6f\r\n", gps.gps_latitude, gps.gps_longitude, baro.baro_altitude);


		   //		   //mag = sensors.GetMagData();
		   //		   gyro = sensors.GetGyroData();
//		   sprintf(logBuf, "ax:%.7f, ay:%.7f, az:%.7f\r\n",acc.x, acc.y, acc.z);
//		   //sprintf(logBuf, "ax:%.7f, ay:%.7f, az:%.7f\r\n",mag.x, mag.y, mag.z);
//		   sprintf(logBuf, "ax:%.7f, ay:%.7f, az:%.7f\r\n",gyro.x, gyro.y, gyro.z);
//
//		 // serializer.SendData(sensors.GetGyroData());
//		CDC_Transmit_FS((uint8_t *) logBuf, strlen(logBuf));
//		   HAL_Delay(500);
		usb_timer = HAL_GetTick();

	  }
  }
}

///**
//  * @brief System Clock Configuration
//  * @retval None
//  */
void SystemClock_Config(void)
{
  // Defective warning for missing initialiser member
  RCC_OscInitTypeDef RCC_OscInitStruct = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0,0,0,0,0,0,0,0};

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
//
///* USER CODE BEGIN 4 */
//
///* USER CODE END 4 */
//
///* MPU Configuration */
//
void MPU_Config(void)
{
//  MPU_Region_InitTypeDef MPU_InitStruct = {0,0,0,0,0,0,0,0,0,0,0};
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
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
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
    	frkskyRC.UpdateSbus(sbus_buffer);
    }
  }

}

//void HAL_HSEM_FreeCallback(uint32_t SemMask)
//{
//  //Notif_Recieved = 1;
//	notify_cm4 |= SemMask;
//}
//
//
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
//
///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
