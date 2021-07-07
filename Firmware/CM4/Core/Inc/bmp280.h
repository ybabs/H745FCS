/*
 * bmp280.h
 *
 *  Created on: Jul 6, 2021
 *      Author: Home
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#include<stdint.h>
#include <stm32h7xx_hal.h>
#include <spi.h>

#define BMP280_CHIP_ID		 	0x58
#define BMP280_REG_ID    		0xD0
#define BMP280_REG_RESET    	0xE0
#define BMP280_REG_STATUS		0xF3
#define BMP280_REG_CTRL_MEAS	0xF4
#define BMP280_REG_CONFIG		0xF5
#define BMP280_REG_PRESS_MSB    0xF7 // 19:12
#define BMP280_REG_PRESS_LSB    0xF8 // 4:11
#define BMP280_REG_PRESS_XLSB   0xF9  //bits 4 - 7
#define BMP280_REG_TEMP_MSB     0xFA
#define BMP280_REG_TEMP_LSB     0xFB
#define BMP280_REG_TEMP_XLSB    0xFC
#define BMP280_RESET_VALUE  0xB6   // readout value is always 0x00
#define SPI_READ            0x80
#define SPI_WRITE           0X7F



typedef enum{

	IM_UPDATE = 0x01,
	MEASURING = 0x08

}BMP280_Status;


typedef enum{
	SKIPPED = 0x00, // No measurement
	X1 = 0x01, // x 2
	X2 = 0x02,
	X4 = 0x03,
	X8 = 0x04,
	X16 = 0x05

}BMP280_Oversampling;

typedef enum{
	SLEEP = 0x00,
	FORCED = 0x01,
	NORMAL = 0x03
}BMP280_Mode;


typedef enum{
	FILTER_OFF = 0x00,
	FILTER_2 = 0x01,
	FILTER_4 = 0x02,
	FILTER_8 = 0x03,
	FILTER_16 = 0x04
}BMP280_Filter;

typedef enum{
	STANDBY_0_5 = 0x00, // 0.5 ms
	STANDBY_62_5 = 0x01, //62.5ms
	STANDBY_125 = 0x02, //125 ms
	STANDBY_250 = 0x03, // 250ms
	STANDBY_500 = 0x04, // 500 ms
	STANDBY_1000 = 0x05, // 1000ms
	STANDBY_2000 = 0x06, // 2000ms
	STANDBY_4000 = 0x07  // 4000ms

}BMP280_StandbyTime;

typedef struct{
	BMP280_Mode mode;
	BMP280_Filter IIR_Filter;
	BMP280_StandbyTime standby;
	BMP280_Oversampling pressure_oversampling;
	BMP280_Oversampling temp_oversampling;
}BMP280_config;

typedef enum{
	 dig_T1 = ((uint16_t) 0x89),
	 dig_T2 = ((uint16_t) 0x8B),
	 dig_T3 = ((uint16_t) 0x8D),
	 dig_P1 = ((uint16_t) 0x8F),
	 dig_P2 = ((uint16_t) 0x91),
	 dig_P3 = ((uint16_t) 0x93),
	 dig_P4 = ((uint16_t) 0x95),
	 dig_P5 = ((uint16_t) 0x97),
	 dig_P6 = ((uint16_t) 0x99),
	 dig_P7 = ((uint16_t) 0x9B),
	 dig_P8 = ((uint16_t) 0x9D),
	 dig_P9 = ((uint16_t) 0x9F)

}BMP280_CompensationParam;

typedef struct{
	uint16_t dig_T1;
	uint16_t dig_T2;
	uint16_t dig_T3;
	uint16_t dig_P1;
	uint16_t dig_P2;
	uint16_t dig_P3;
	uint16_t dig_P4;
	uint16_t dig_P5;
	uint16_t dig_P6;
	uint16_t dig_P7;
	uint16_t dig_P8;
	uint16_t dig_P9;
}BMP280_CompensationValues;


typedef struct{
	float temperature;
	float pressure;
	BMP280_config config;
	BMP280_CompensationValues compensation_vals;
}BMP280Sensor;



HAL_StatusTypeDef BMP280InitParams(BMP280_config* config);
HAL_StatusTypeDef BMP280InitDevice(BMP280Sensor* sensor);
HAL_StatusTypeDef BMP280ReadCompensationData(BMP280Sensor* sensor);
int32_t BMP280ReadUP(BMP280Sensor* sensor);
int32_t BMP280ReadUT(BMP280Sensor* sensor);
uint8_t BMP280ReadSensorID();
uint8_t BMP280_ReadReg8(uint8_t address, uint8_t* data);
uint8_t BMP280_WriteReg8(uint8_t address, uint8_t* data);


uint8_t spi_rxBuffer [8];



#endif /* INC_BMP280_H_ */
