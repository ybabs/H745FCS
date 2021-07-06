#ifndef COMMON_H
#define COMMON_H

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"





/* LED DEFINES*/
#define LED_A_Pin 			GPIO_PIN_7
#define LED_A_GPIO_Port 	GPIOE
#define LED_B_Pin 			GPIO_PIN_8
#define LED_B_GPIO_Port 	GPIOE
#define LED_C_Pin 			GPIO_PIN_8
#define LED_C_GPIO_Port 	GPIOF
#define LED_D_Pin 			GPIO_PIN_9
#define LED_D_GPIO_Port 	GPIOF



#define LEDA_ON()  HAL_GPIO_WritePin(LED_A_GPIO_Port, LED_A_Pin, GPIO_PIN_SET)
#define LEDA_OFF() HAL_GPIO_WritePin(LED_A_GPIO_Port, LED_A_Pin, GPIO_PIN_RESET)


#define LEDB_ON()  HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET)
#define LEDB_OFF() HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET)


#define LEDC_ON()  HAL_GPIO_WritePin(LED_C_GPIO_Port, LED_C_Pin, GPIO_PIN_SET)
#define LEDC_OFF() HAL_GPIO_WritePin(LED_C_GPIO_Port, LED_C_Pin, GPIO_PIN_RESET)


#define LEDD_ON()  HAL_GPIO_WritePin(LED_D_GPIO_Port, LED_D_Pin, GPIO_PIN_SET)
#define LEDD_OFF() HAL_GPIO_WritePin(LED_D_GPIO_Port, LED_D_Pin, GPIO_PIN_RESET)


/*LSM9DS1 Defines */
#define LSM9DS1_CS_M_Pin		GPIO_PIN_4
#define LSM9DS1_CS_M_Port  		GPIOB
#define LSM9DS1_DRDY_M_Pin 		GPIO_PIN_14
#define LSM9DS1_DRDY_M_Port 	GPIOG
#define LSM9DS1_INT_M_Pin 		GPIO_PIN_13
#define LSM9DS1_INT_M_Port  	GPIOG
#define LSM9DS1_INT1_AG_Pin		GPIO_PIN_12
#define LSM9DS1_INT1_AG_Port  	GPIOG
#define LSM9DS1_INT2_AG_Pin		GPIO_PIN_11
#define LSM9DS1_INT2_AG_Port  	GPIOG
#define LSM9DS1_DEN_AG_Pin		GPIO_PIN_10
#define LSM9DS1_DEN_AG_Port  	GPIOG
#define LSM9DS1_SDO_M_Pin		GPIO_PIN_2
#define LSM9DS1_SDO_M_Port  	GPIOE
#define LSM9DS1_SDO_AG_Pin		GPIO_PIN_4
#define LSM9DS1_SDO_AG_Port  	GPIOE
#define LSM9DS1_CS_AG_Pin		GPIO_PIN_3
#define LSM9DS1_CS_AG_Port  	GPIOE

// SBUS RX Port
#define SBUS_RX_Pin  			GPIO_PIN_1
#define SBUS_RX_Port 			GPIOA


// define SPI NCS
#define SPI1_NCS_Pin 			GPIO_PIN_4
#define SPI1_NCS_Port 			GPIOA



#define GPS_NRST_Pin 			GPIO_PIN_15
#define GPS_NRST_Port			GPIOD
#define GPS_PPS_Pin				GPIO_PIN_14
#define GPS_PPS_Port			GPIOD











#endif
