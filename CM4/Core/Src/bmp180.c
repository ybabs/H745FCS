#include "bmp180.h"
#include "common.h"


HAL_StatusTypeDef BMP180InitDevice(bmp180Sensor* sensor)
{
	HAL_StatusTypeDef status;

	uint8_t chip_id = 0x00;

	chip_id = BaroReadSensorID(BMP180_READ_ADDRESS, CHIP_ID);

	if(chip_id == I_AM_BMP180)
	{
		BMPReadCalibCoefficients(sensor);
		sensor->oss_mode = OSS_HIGHRESOLUTION;
		LED1_ON();
		status = HAL_OK;
	}

	else
	{
		ErrorHandler();
		status = HAL_ERROR;
	}

	return status;
}

HAL_StatusTypeDef BMPReadCalibCoefficients(bmp180Sensor* sensor)
{
	HAL_StatusTypeDef status;
	uint8_t coeff_buf[BMP180_COEFF_DATASIZE] = {0};
	uint8_t i = 0;


	// Read contents of register
	status = HAL_I2C_Mem_Read(&hi2c1, BMP180_READ_ADDRESS, AC1, 1, coeff_buf, BMP180_COEFF_DATASIZE, 1000);

	if(status != HAL_OK)
	{
		ErrorHandler();
	}

	//check that device coefficients are correct
	for(i = 0; i < BMP180_COEFF_DATASIZE/2; i++)
	{
		uint16_t coeff = (coeff_buf[2*i] << 8) | coeff_buf[2 * i + 1];
		//check coefficient values
		if(coeff == 0 || coeff == 0xFFFF)
		{
			status = HAL_ERROR;
			ErrorHandler();
		}
	}

	sensor->coefficients.ac1 = coeff_buf[0] << 8 | coeff_buf[1];
	sensor->coefficients.ac2 = coeff_buf[2] << 8 | coeff_buf[3];
	sensor->coefficients.ac3 = coeff_buf[4] << 8 | coeff_buf[5];
	sensor->coefficients.ac4 = coeff_buf[6] << 8 | coeff_buf[7];
	sensor->coefficients.ac5 = coeff_buf[8] << 8 | coeff_buf[9];
	sensor->coefficients.ac6 = coeff_buf[10] << 8 | coeff_buf[11];
	sensor->coefficients.b1 = coeff_buf[12] << 8 | coeff_buf[13];
	sensor->coefficients.b2 = coeff_buf[14] << 8 | coeff_buf[15];
	sensor->coefficients.mb = coeff_buf[16] << 8 | coeff_buf[17];
	sensor->coefficients.mc = coeff_buf[18] << 8 | coeff_buf[19];
	sensor->coefficients.md = coeff_buf[20] << 8 | coeff_buf[21];


	return status;

}

int32_t BMP180ReadUP(bmp180Sensor* sensor)
{
	HAL_StatusTypeDef status;
	uint8_t oss = sensor->oss_mode;
	uint8_t output[3];
	uint8_t cmd = PRESSURE_CMD + (oss << 6);
    I2CWrite(BMP180_WRITE_ADDRESS, REG_CTRL, cmd);

	switch (oss)
	{
	case 0:
		HAL_Delay(PRESSURE_0_CONV_TIME);
		break;
	case 1:
		HAL_Delay(PRESSURE_1_CONV_TIME);
		break;
	case 2:
		HAL_Delay(PRESSURE_2_CONV_TIME);
		break;
	case 3:
		HAL_Delay(PRESSURE_3_CONV_TIME);
		break;
	}

	status = HAL_I2C_Mem_Read(&hi2c1, BMP180_READ_ADDRESS, REG_PRESSURE, 1, output, sizeof(output), 1000);
	if(status != HAL_OK)
	{
		ErrorHandler();
	}

	int32_t raw_pressure = ((uint32_t) output[0] << 16 | output[1] << 8 | output[2]) >> (8 - oss);

	return raw_pressure;

}

int32_t BMP180ReadUT()
{
	HAL_StatusTypeDef status;
	uint8_t cmd = TEMPERATURE_CMD;
	uint8_t output[2];
	//Write command to temperature
	I2CWrite(BMP180_WRITE_ADDRESS, REG_CTRL, cmd);
	// Wait 4.5ms
	HAL_Delay(TEMP_CONV_TIME);
	status = HAL_I2C_Mem_Read(&hi2c1, BMP180_READ_ADDRESS, REG_TEMP, 1, output, sizeof(output), 1000);
	if(status != HAL_OK)
	{
		ErrorHandler();
	}

	int32_t raw_temp = (uint32_t) output[0] << 8 | output[1];

	return raw_temp;

}


void BMP180ReadTemperature(bmp180Sensor* sensor)
{
	int32_t x1 = 0;
	int32_t x2 = 0;

	int32_t ut = BMP180ReadUT();

	uint16_t ac6 = sensor->coefficients.ac6;
	uint16_t ac5 = sensor->coefficients.ac5;
	int16_t mc = sensor->coefficients.mc;
	int16_t md = sensor->coefficients.md;

	x1 = ((ut - ac6) * ac5) >> 15;
	x2 = ((int32_t) mc << 11) / (x1 + md);
	sensor->b5 = x1 + x2;
	sensor->temperature = ((sensor->b5 + 8) >> 4) * 0.1f; // multiply by 0.1 as temp is in degrees

}

/*
 * @return - pressure in Pascal
 */
void BMP180ReadPressue(bmp180Sensor* sensor)
{
  int32_t x1, x2, x3, p, b3, b6 = 0;
  uint32_t b4, b7 = 0;
  int16_t b2 = sensor->coefficients.b2;
  int16_t ac2 = sensor->coefficients.ac2;
  int16_t ac1 = sensor->coefficients.ac1;
  int16_t ac3 = sensor->coefficients.ac3;
  int16_t b1 = sensor->coefficients.b1;
  uint16_t ac4 = sensor->coefficients.ac4;
  uint8_t oss = sensor->oss_mode;

  // read uncompensated pressure
  int32_t up = BMP180ReadUP(sensor);


  b6 = sensor->b5 - 4000;
  x1 = (b2 * ((b6 * b6) >> 12)) >> 11;
  x2 = (ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((ac1 * 4 + x3) << sensor->oss_mode) + 2) >> 2;
  x1 = (ac3 * b6) >> 13;
  x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = (x1 + x2 + 2) >> 2;
  b4 =  (ac4 * (unsigned long)(x3 + 32768)) >> 15;
  b7 = ((unsigned long) up - b3) * (50000 >> oss);

  if (b7 < 0x80000000)
  {
	  p = (b7 * 2) / b4;
  }

  else
  {
	  p = (b7 / b4) * 2;
  }

  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;

  sensor->pressure = p + (x1 + x2 + 3791) / 16.0 ;

}

void BMP180ReadAltitude(bmp180Sensor* sensor)
{
	float altitude = 0;
	float pressure = sensor->pressure;

	altitude = PRESSURE_COEFFICENT * (1.0f - pow((pressure/ SEA_LEVEL_PRESSURE), (1/5.255)));
	//altitude =  (1.0f - pow((pressure/ SEA_LEVEL_PRESSURE), (1/5.255)));

	if((altitude <= MIN_ALTITUDE) || altitude >= MAX_ALTITUDE)
	{
		ErrorHandler();
	}

	sensor->altitude = altitude;

}



uint8_t BaroReadSensorID (uint16_t DeviceAddr, uint8_t reg)
{
	return I2CRead(DeviceAddr, reg);
}
