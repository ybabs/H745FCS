#ifndef BMP_180_H
#define BMP_180_H

#include <stdint.h>
#include<stm32h7xx_hal.h>
#include "i2c.h"

#define BMP180_WRITE_ADDRESS 0xEE
#define BMP180_READ_ADDRESS 0xEF
#define I_AM_BMP180 0x55
#define BMP180_COEFF_DATASIZE  22U  // (0xBF - 0xAA ) + 1

#define	TEMP_CONV_TIME 5
#define	PRESSURE_0_CONV_TIME 5
#define	PRESSURE_1_CONV_TIME 8
#define	PRESSURE_2_CONV_TIME 14
#define	PRESSURE_3_CONV_TIME 26

#define SEA_LEVEL_PRESSURE (101325.0f) // in Pa from 1013.25hPa
#define PRESSURE_COEFFICENT (44330.0f)
#define MAX_ALTITUDE (9000) // in metres above sea-level
#define MIN_ALTITUDE (-500) // in metres

//16 bit addresses
typedef enum{
	AC1 = ((uint16_t) 0xAA),
	AC2 = ((uint16_t) 0xAC),
	AC3 = ((uint16_t) 0xAE),
	AC4 = ((uint16_t) 0xB0),
	AC5 = ((uint16_t) 0xB2),
	AC6 = ((uint16_t) 0xB4),
	B1 = ((uint16_t) 0xB6),
	B2 = ((uint16_t) 0xB8),
	MB = ((uint16_t) 0xBA),
	MC = ((uint16_t) 0xBC),
	MD = ((uint16_t) 0xBE)
}BMPCoefficentAddresses;

typedef enum{
	TEMPERATURE_CMD = 0x2E,
	PRESSURE_CMD = 0x34
}Commands;

typedef enum{
	TEMP_CONV =  ((int16_t) 4500),
	PRESSURE_0_CONV = ((int16_t) 4500),
	PRESSURE_1_CONV = ((int16_t) 7500),
	PRESSURE_2_CONV = ((int16_t) 13500),
	PRESSURE_3_CONV = ((int16_t) 25500)
}ConversionMS;

typedef enum
{
	OSS_ULTRALOWPOWER = 0x00,
	OSS_STANDARD =  0x01,
	OSS_HIGHRESOLUTION = 0x02,
	OSS_ULTRAHIGHRESOLUTION = 0x03

}BMP_180Mode;

typedef enum
{
	REG_CTRL = 0xF4,
	REG_TEMP = 0xF6,
	REG_PRESSURE = 0xF6,
	CHIP_ID = 0xD0
}Readouts;

typedef struct bmp180Coefficients
{
  int16_t ac1;
  int16_t ac2;
  int16_t ac3;
  uint16_t ac4;
  uint16_t ac5;
  uint16_t ac6;
  int16_t b1;
  int16_t b2;
  int16_t mb;
  int16_t mc;
  int16_t md;
} bmp180Coefficients;


typedef struct bmp180Sensor{
	float altitude;
	float pressure;
	float temperature;
	int32_t b5;
	BMP_180Mode oss_mode;
	bmp180Coefficients coefficients;
}bmp180Sensor;


HAL_StatusTypeDef BMP180InitDevice(bmp180Sensor* sensor);
HAL_StatusTypeDef BMPReadCalibCoefficients(bmp180Sensor* sensor);
int32_t BMP180ReadUP(bmp180Sensor* sensor);
int32_t BMP180ReadUT();
void BMP180ReadTemperature(bmp180Sensor* sensor);
void BMP180ReadPressue(bmp180Sensor* sensor);
void BMP180ReadAltitude(bmp180Sensor* sensor);
uint8_t BaroReadSensorID (uint16_t DeviceAddr, uint8_t reg);
//Todo Sealevel Pressure??

#endif
