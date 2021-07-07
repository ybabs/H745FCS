#include "bmp280.h"
#include "common.h"


uint8_t BMP280ReadSensorID()
{
	uint8_t data = 0;
	uint8_t status = BMP280_ReadReg8(BMP280_REG_ID, &data);
	if(status == HAL_OK)
	{
		return data;
	}

	else
	{
		return HAL_ERROR;
	}

}


uint8_t BMP280_ReadReg8(uint8_t address, uint8_t* data)
{
	// Pull Pin low initially
	uint8_t txBuf[2] = {(0xD0|SPI_READ), 0x00};
	uint8_t rx_buff[2];
	HAL_GPIO_WritePin(SPI1_NCS_Port, SPI1_NCS_Pin, GPIO_PIN_RESET);
	if(HAL_SPI_TransmitReceive_DMA(&hspi1, txBuf, rx_buff, 2) == HAL_OK)
	{
        *data = rx_buff[1];
        return HAL_OK;
	}

	else
	{
		HAL_GPIO_WritePin(SPI1_NCS_Port, SPI1_NCS_Pin, GPIO_PIN_SET);
		return HAL_ERROR;
	}
}
HAL_StatusTypeDef BMP280_WriteReg8(uint8_t address, uint8_t* data)
{

}


HAL_StatusTypeDef BMP280InitParams(BMP280_config* config)
{

}
HAL_StatusTypeDef BMP280InitDevice(BMP280Sensor* sensor)
{

}
HAL_StatusTypeDef BMP280ReadCompensationData(BMP280Sensor* sensor)
{

}
int32_t BMP280ReadUP(BMP280Sensor* sensor)
{

}
int32_t BMP280ReadUT(BMP280Sensor* sensor)
{

}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    //while(1);
}

