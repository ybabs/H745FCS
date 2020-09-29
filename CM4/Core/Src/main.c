/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "cmsis_os.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "common.h"

#include "ublox.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t data_byte[1] = {0};
uint8_t error_code = 0;

struct NAV_PVT pvt;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void MX_FREERTOS_Init(void);
void ConfigGPS(void);
void ConfigPWMChannels(void);
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
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  ConfigGPS();
  ConfigPWMChannels();

  if(HAL_UART_Receive_DMA(&huart1, (uint8_t *)data_byte, 1)== HAL_OK)
  {
	  	LED2_ON();
  }
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void ConfigGPS(void)
{
		if(HAL_UART_Transmit_DMA(&huart1, (uint8_t *)UBLOX_10HZ_RATE, sizeof(UBLOX_10HZ_RATE)) == HAL_OK)
		{
		  LED3_ON();
		}
		HAL_Delay(100);

	    if(HAL_UART_Transmit_DMA(&huart1, (uint8_t *)UBLOX_GXGGA_OFF, sizeof(UBLOX_GXGGA_OFF)) == HAL_OK)
	    {
	  	  LED3_ON();
	    }
	    HAL_Delay(100);

	    if(HAL_UART_Transmit_DMA(&huart1, (uint8_t *)UBLOX_GXGLL_OFF, sizeof(UBLOX_GXGLL_OFF)) == HAL_OK)
	    {
	  	  LED3_ON();
	    }
	    HAL_Delay(100);

	    if(HAL_UART_Transmit_DMA(&huart1, (uint8_t *)UBLOX_GXGSA_OFF, sizeof(UBLOX_GXGSA_OFF)) == HAL_OK)
	    {
	  	  LED3_ON();
	    }
	    HAL_Delay(100);

	    if(HAL_UART_Transmit_DMA(&huart1, (uint8_t *)UBLOX_GXGSV_OFF, sizeof(UBLOX_GXGSV_OFF)) == HAL_OK)
	    {
	  	  LED3_ON();
	    }
	    HAL_Delay(100);

	    if(HAL_UART_Transmit_DMA(&huart1, (uint8_t *)UBLOX_GXRMC_OFF, sizeof(UBLOX_GXRMC_OFF)) == HAL_OK)
	    {
	  	  LED3_ON();
	    }
	    HAL_Delay(100);

	    if(HAL_UART_Transmit_DMA(&huart1, (uint8_t *)UBLOX_GXVTG_OFF, sizeof(UBLOX_GXVTG_OFF)) == HAL_OK)
	    {
	  	  LED3_ON();
	    }
	    HAL_Delay(100);

	    if(HAL_UART_Transmit_DMA(&huart1, (uint8_t *)UBLOX_NAVPVT_ON, sizeof(UBLOX_NAVPVT_ON)) == HAL_OK)
	    {
	  	  LED3_ON();
	    }
}

void ConfigPWMChannels(void)
{
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static int byte_pos = 0;
	static unsigned char checksum[2];
	const int data_size = sizeof(pvt);

	if(huart->Instance == USART1)
	{
		if(byte_pos < 2)
		{
			if(data_byte[0] == UBX_HEADER[byte_pos])
			{
				byte_pos++;
			}
			else
			{
				byte_pos = 0;
			}
		}

		else
		{
			if( (byte_pos-2) < data_size)
			{
				((unsigned char*)(&pvt))[byte_pos-2] = data_byte[0];
			}

			byte_pos++;

			if ( byte_pos == (data_size+2) )
			{
			        calcChecksum(checksum);
			}

			else if ( byte_pos == (data_size+3) )
			{
				if ( data_byte[0] != checksum[0] )
					byte_pos = 0;
			}

		      else if ( byte_pos == (data_size+4) ) {
		    	  byte_pos = 0;
		        if ( data_byte[0] == checksum[1] ) {
		        	LED2_ON();

		        }
		      }

		      else if ( byte_pos > (data_size+4) ) {
		    	  byte_pos = 0;
		      }
		}
	}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
   if(huart->Instance == USART1)
	 {
	   LED2_ON();
	 }
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Initialization Error */
  LED1_ON();
  error_code = huart->ErrorCode;

}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
