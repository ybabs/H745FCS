#ifndef COMMON_H
#define COMMON_H

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

#define GPS_DATA_TYPE 0x01
#define MAG_DATA_TYPE 0x02
#define ACC_DATA_TYPE 0x03
#define GYRO_DATA_TYPE 0x04
#define BARO_DATA_TYPE 0x05

// Update Rates of sensors in milliseconds
#define GPS_SAMPLE_TIME =  100; // GPS has 10HZ update rate
#define MAG_SAMPLE_TIME =  13; // 80 Hz
#define IMU_SAMPLE_TIME =  1; // 952HZ
#define BARO_SAMPLE_TIME = 38; // 26.3 Hz

#define MEM_ALIGN(x)   (((x) + 0x00000003) & ~0x00000003)

#define SRAM4_ADDR 0x38000000
#define SRAM4_LEN  0x0000FFFF
#define IMU_PACKET_LEN (MEM_ALIGN(sizeof(LSM9DS1Handle)))
#define GPS_PACKET_LEN (MEM_ALIGN(sizeof(GPSHandle)))
#define BARO_PACKET_LEN (MEM_ALIGN(sizeof(BMP280Handle)))

// define Addresses
#define M4_TO_M7_ADDR MEM_ALIGN(SRAM4_ADDR)
#define IMU_BUFFER      MEM_ALIGN(M4_TO_M7_ADDR + IMU_PACKET_LEN)

#define US_IN_SECOND            ((uint32_t)1000000)
#define TimerCount_Start()  do{\
              SysTick->LOAD  =  0xFFFFFF  ; /* set reload register */\
              SysTick->VAL  =  0  ;     /* Clear Counter */    \
              SysTick->CTRL  =  0x5 ;     /* Enable Counting*/   \
              }while(0)

#define TimerCount_Stop(Value)  do {\
                SysTick->CTRL  =0;  /* Disable Counting */         \
                Value = SysTick->VAL;/* Load the SysTick Counter Value */\
                Value = 0xFFFFFF - Value;/* Capture Counts in CPU Cycles*/\
                }while(0)


// LED Defines
/* LED DEFINES*/
#define LED_A_Pin       GPIO_PIN_7
#define LED_A_GPIO_Port   GPIOE
#define LED_B_Pin       GPIO_PIN_8
#define LED_B_GPIO_Port   GPIOE
#define LED_C_Pin       GPIO_PIN_8
#define LED_C_GPIO_Port   GPIOF
#define LED_D_Pin       GPIO_PIN_9
#define LED_D_GPIO_Port   GPIOF

#define LEDA_ON()  HAL_GPIO_WritePin(LED_A_GPIO_Port, LED_A_Pin, GPIO_PIN_SET)
#define LEDA_OFF() HAL_GPIO_WritePin(LED_A_GPIO_Port, LED_A_Pin, GPIO_PIN_RESET)

#define LEDB_ON()  HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET)
#define LEDB_OFF() HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET)

#define LEDC_ON()  HAL_GPIO_WritePin(LED_C_GPIO_Port, LED_C_Pin, GPIO_PIN_SET)
#define LEDC_OFF() HAL_GPIO_WritePin(LED_C_GPIO_Port, LED_C_Pin, GPIO_PIN_RESET)

#define LEDD_ON()  HAL_GPIO_WritePin(LED_D_GPIO_Port, LED_D_Pin, GPIO_PIN_SET)
#define LEDD_OFF() HAL_GPIO_WritePin(LED_D_GPIO_Port, LED_D_Pin, GPIO_PIN_RESET)


// SBUS RX Port
#define SBUS_RX_Pin       GPIO_PIN_1
#define SBUS_RX_Port      GPIOA

// GPS Port
#define GPS_NRST_Pin      GPIO_PIN_15
#define GPS_NRST_Port     GPIOD
#define GPS_PPS_Pin       GPIO_PIN_14
#define GPS_PPS_Port      GPIOD

// Barometer NCS
#define BMP280_NCS_Port       GPIOA
#define BMP280_NSS_Pin        GPIO_PIN_4


/*
 * Accelerometer data
 */
 struct acc_data
 {
     float imu_acc_x;
     float imu_acc_y;
     float imu_acc_z;
 };

 struct gyro_data
 {
     float imu_gyro_x;
     float imu_gyro_y;
     float imu_gyro_z;
 };


 struct mag_data
 {
     float imu_mag_x;
     float imu_mag_y;
     float imu_mag_z;
 };

 struct gps_data
 {
     float gps_latitude;
     float gps_longitude;
     float gps_altitude;
     float gps_velocity_x;
     float gps_velocity_y;
     float gps_velocity_z;
     float gnd_speed;
     int   gps_satellites;
 };

 struct baro_data
 {
     float baro_pressure;
     float baro_altitude;
 };

 struct quaternion
 {
   float q0;
   float q1;
   float q2;
   float q3;
 };

#define RAD_TO_DEG   57.2957795131f
#define DEG_TO_RAD   0.01745329252f
#define GAUSS_TO_MICROTESLA  100

#define PI           3.141592653589793f

#endif // COMMON_H
