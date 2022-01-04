#ifndef BMP280_H
#define BMP280_H

#include<stdint.h>
#include <stm32h7xx_hal.h>
#include <spi.h>

#define BMP280_CHIP_ID         0x58
#define BMP280_REG_ID          0xD0
#define BMP280_REG_RESET       0xE0
#define BMP280_REG_STATUS      0xF3
#define BMP280_REG_CTRL_MEAS   0xF4
#define BMP280_REG_CONFIG      0xF5
#define BMP280_REG_PRESS_MSB   0xF7 // 19:12
#define BMP280_REG_PRESS_LSB   0xF8 // 4:11
#define BMP280_REG_PRESS_XLSB  0xF9  //bits 4 - 7
#define BMP280_REG_TEMP_MSB    0xFA
#define BMP280_REG_TEMP_LSB    0xFB
#define BMP280_REG_TEMP_XLSB   0xFC
#define BMP280_RESET_VALUE     0xB6   // readout value is always 0x00
#define BMP280_SPI_READ        0x80
#define BMP280_SPI_WRITE       0X7F

#define SEA_LEVEL_PRESSURE (101325.0f) // in Pa from 1013.25hPa
#define PRESSURE_COEFFICENT (44330.0f)
#define MAX_ALTITUDE (9000) // in metres above sea-level
#define MIN_ALTITUDE (-500) // in metres

typedef struct
{
  uint16_t dig_T1;
  int16_t dig_T2;
  int16_t dig_T3;
  uint16_t dig_P1;
  int16_t dig_P2;
  int16_t dig_P3;
  int16_t dig_P4;
  int16_t dig_P5;
  int16_t dig_P6;
  int16_t dig_P7;
  int16_t dig_P8;
  int16_t dig_P9;
} BMP280_CompensationValues;

typedef enum
{
  dig_T1 = ((uint16_t) 0x88),
  dig_T2 = ((uint16_t) 0x8A),
  dig_T3 = ((uint16_t) 0x8C),
  dig_P1 = ((uint16_t) 0x8E),
  dig_P2 = ((uint16_t) 0x90),
  dig_P3 = ((uint16_t) 0x92),
  dig_P4 = ((uint16_t) 0x94),
  dig_P5 = ((uint16_t) 0x96),
  dig_P6 = ((uint16_t) 0x98),
  dig_P7 = ((uint16_t) 0x9A),
  dig_P8 = ((uint16_t) 0x9C),
  dig_P9 = ((uint16_t) 0x9E)

} BMP280_CompensationParam;

typedef enum
{

  IM_UPDATE = 0x01,
  MEASURING = 0x08

} BMP280_Status;

typedef enum
{
  SKIPPED = 0x00, // No measurement
  X1 = 0x01, // x 2
  X2 = 0x02,
  X4 = 0x03,
  X8 = 0x04,
  X16 = 0x05

} BMP280_Oversampling;

typedef enum
{
  SLEEP = 0x00,
  FORCED = 0x01,
  NORMAL = 0x03
} BMP280_Mode;

typedef enum
{
  FILTER_OFF = 0x00,
  FILTER_2 = 0x01,
  FILTER_4 = 0x02,
  FILTER_8 = 0x03,
  FILTER_16 = 0x04
} BMP280_Filter;

typedef enum
{
  STANDBY_0_5 = 0x00, // 0.5 ms
  STANDBY_62_5 = 0x01, //62.5ms
  STANDBY_125 = 0x02, //125 ms
  STANDBY_250 = 0x03, // 250ms
  STANDBY_500 = 0x04, // 500 ms
  STANDBY_1000 = 0x05, // 1000ms
  STANDBY_2000 = 0x06, // 2000ms
  STANDBY_4000 = 0x07  // 4000ms

} BMP280_StandbyTime;

typedef struct
{
  BMP280_Mode mode;
  BMP280_Filter IIR_Filter;
  BMP280_StandbyTime standby;
  BMP280_Oversampling pressure_oversampling;
  BMP280_Oversampling temp_oversampling;
} BMP280_Config;

typedef struct
{
  float temperature;
  float pressure;
  float altitude;

  BMP280_Config config;
  BMP280_CompensationValues comp_vals;

} BMP280Handle;

int32_t t_fine;
uint8_t SerialData[3];

uint8_t ConfigBMP280(BMP280Handle *baro);
void ResetBMP280(void);
uint8_t CheckBMP280ChipID();
uint8_t Read8Bit(uint8_t Register);
uint16_t Read16Bit(uint8_t Register);
uint32_t Read24Bit(uint8_t Register);
void Write8Bit(uint8_t Register, uint8_t Data);
uint16_t Read16BitLE(uint8_t reg);
void BMP280readCoefficients(BMP280Handle *baro);
void SetConfig(BMP280Handle *baro);
void ReadTemp(BMP280Handle *baro);
void ReadPressure(BMP280Handle *baro);
void ReadAltitude(BMP280Handle *baro);
void SetBMP280NSS(void);
void ResetBMP280NSS(void);



#endif //BMP280_H
