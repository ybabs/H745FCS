#include "bmp280.h"
#include <string.h>

int32_t t_fine;
uint8_t SerialData[3];
/*
 * @brief Pulls the NCS pin of the BMP280 High
 * @returns nothing
 */
void SetBMP280NSS(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

}
/*
 * @brief Pulls the NCS pin of the BMP280 Low
 * @returns nothing
 */
void ResetBMP280NSS(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

/*
 * @brief Checks the WHOAMI register
 * to confirm the chip
 * @returns OK on success, Error on failure
 */
uint8_t CheckBMP280ChipID()
{
  if (Read8Bit(BMP280_REG_ID) != BMP280_CHIP_ID)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

/*
 * @brief Initialises BMP280 Sensor
 * @retval HAL_OK if initialisation is successful
 */
uint8_t ConfigBMP280(BMP280Handle *baro)
{
  ResetBMP280NSS();
  uint8_t baro_res = CheckBMP280ChipID();
  if (baro_res != HAL_OK)
  {
    return HAL_ERROR;
  }

  ResetBMP280NSS();
  HAL_Delay(1000);
  SetConfig(baro);
  BMP280readCoefficients(baro);

  return HAL_OK;

}

/**
 * @brief Reads 8 bit register.
 * @param The register adddress to be read.
 * @return 8 bit data from the register.
 */
uint8_t Read8Bit(uint8_t Register)
{
  SerialData[0] = Register | BMP280_SPI_READ;

  ResetBMP280NSS();
  HAL_SPI_Transmit(&hspi1, SerialData, 1, 10);
  HAL_SPI_Receive(&hspi1, &SerialData[1], 1, 10);
  SetBMP280NSS();
  return SerialData[1];

}
/**
 * @brief Reads 2 consecutive 8 bit register.
 * @param Register The starting address of register to be read.
 * @return the 16 bit obtained after reading 2 registers.
 */
uint16_t Read16Bit(uint8_t Register)
{
  uint32_t Out, msb, lsb;
  SerialData[0] = Register | BMP280_SPI_READ;


  ResetBMP280NSS();
  HAL_SPI_Transmit(&hspi1, SerialData, 1, 10);
  HAL_SPI_Receive(&hspi1, &SerialData[0], 2, 10);
  SetBMP280NSS();
  msb = (SerialData[0] << 8);
  lsb = (SerialData[1]);
  Out = msb | lsb;
  return Out;
}

/**
 * @brief Reads 3 consecutive 8 bit registers.
 * @param Register The starting register address.
 * @return the 24 bit obtained after reading 3 registers.
 */
uint32_t Read24Bit(uint8_t Register)
{
  uint32_t Out, msb, lsb, xlsb;
  SerialData[0] = Register | BMP280_SPI_READ;

  ResetBMP280NSS();
  HAL_SPI_Transmit(&hspi1, SerialData, 1, 10);
  HAL_SPI_Receive(&hspi1, &SerialData[0], 3, 10);
  SetBMP280NSS();
  msb = (SerialData[0] << 16);
  lsb = (SerialData[1] << 8);
  xlsb = (SerialData[2] >> 4);
  Out = msb | lsb | xlsb;
  return Out;
}

/**
 * @brief Reads 8 bit data from register.
 * @param Register The 8 bit register address.
 * @param Data The data to be written to the register.
 */
void Write8Bit(uint8_t Register, uint8_t Data)
{
  SerialData[0] = Register & BMP280_SPI_WRITE;
  SerialData[1] = Data;
  ResetBMP280NSS();
  HAL_SPI_Transmit(&hspi1, SerialData, 2, 30);
  SetBMP280NSS();
}


uint16_t Read16BitLE(uint8_t reg)
{
  uint16_t temp = Read16Bit(reg);
  return (temp >> 8) | (temp << 8);
}


/*
 *  @brief  Reads the factory-set coefficients
 */
void BMP280readCoefficients(BMP280Handle *baro)
{
  baro->comp_vals.dig_T1 = Read16BitLE(dig_T1);
  baro->comp_vals.dig_T2 = Read16BitLE(dig_T2);
  baro->comp_vals.dig_T3 = Read16BitLE(dig_T3);
  baro->comp_vals.dig_P1 = Read16BitLE(dig_P1);
  baro->comp_vals.dig_P2 = Read16BitLE(dig_P2);
  baro->comp_vals.dig_P3 = Read16BitLE(dig_P3);
  baro->comp_vals.dig_P4 = Read16BitLE(dig_P4);
  baro->comp_vals.dig_P5 = Read16BitLE(dig_P5);
  baro->comp_vals.dig_P6 = Read16BitLE(dig_P6);
  baro->comp_vals.dig_P7 = Read16BitLE(dig_P7);
  baro->comp_vals.dig_P8 = Read16BitLE(dig_P8);
  baro->comp_vals.dig_P9 = Read16BitLE(dig_P9);
}

/**
 * @brief Sets the sampling rate for both temperature and
 * pressure measurement. Standby Mode and IIR Filter can also be set set by writing
 * appropriate value to the register.The function should be called
 * in order to start measurement of both temp and pressure.
 * @param baro a pointer to the BMP280 struct
 * @returns nothing
 */
void SetConfig(BMP280Handle *baro)
{
  // Ultra High resolution 26.3 Hz
  baro->config.mode = NORMAL;
  baro->config.pressure_oversampling = X16;
  baro->config.temp_oversampling = X2;
  uint8_t oversampling_data;
  oversampling_data = baro->config.temp_oversampling << 5 | baro->config.pressure_oversampling << 2 | baro->config.mode;

  Write8Bit(BMP280_REG_CTRL_MEAS, oversampling_data);

  baro->config.IIR_Filter = FILTER_OFF;
  baro->config.standby = STANDBY_0_5;
  uint8_t filter_config;
  filter_config = baro->config.standby << 5 | baro->config.IIR_Filter;

  Write8Bit(BMP280_REG_CONFIG, filter_config);
}

/*
 * @brief calculates the temperature reported by
 * the BMP280
 * @param baro a pointer to the BMP280 struct
 * @returns nothing
 */
void ReadTemp(BMP280Handle *baro)
{
  int32_t var1, var2;
  int32_t adc_T = Read24Bit(BMP280_REG_TEMP_MSB);
  adc_T >>= 4;

  var1 = ((((adc_T >> 3) - ((int32_t) baro->comp_vals.dig_T1 << 1))) * ((int32_t) baro->comp_vals.dig_T2)) >> 11;

  var2 = (((((adc_T >> 4) - ((int32_t) baro->comp_vals.dig_T1)) * ((adc_T >> 4) - ((int32_t) baro->comp_vals.dig_T1)))
      >> 12) * ((int32_t) baro->comp_vals.dig_T3)) >> 14;

  t_fine = var1 + var2;
  float T = (t_fine * 5 + 128) >> 8;
  baro->temperature = T / 100;

}

/*
 * @brief calculates the pressure reported by
 * the BMP280
 * @param baro a pointer to the BMP280 struct
 * @returns nothing
 */
void ReadPressure(BMP280Handle *baro)
{

  int64_t var1, var2, p;

  // return value of t_fine
  ReadTemp(baro);

  int32_t adc_P = Read24Bit(BMP280_REG_PRESS_MSB);
  adc_P >>= 4;

  var1 = ((int64_t) t_fine) - 128000;
  var2 = var1 * var1 * (int64_t) baro->comp_vals.dig_P6;
  var2 = var2 + ((var1 * (int64_t) baro->comp_vals.dig_P5) << 17);
  var2 = var2 + (((int64_t) baro->comp_vals.dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t) baro->comp_vals.dig_P3) >> 8) + ((var1 * (int64_t) baro->comp_vals.dig_P2) << 12);
  var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) baro->comp_vals.dig_P1) >> 33;

  if (var1 == 0)
  {
    return; // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t) baro->comp_vals.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t) baro->comp_vals.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t) baro->comp_vals.dig_P7) << 4);

  baro->pressure = (float) p / 256;

}

/*
 * @brief Calculates the altitude in metres above
 * sea level.
 * @param baro a pointer to the BMP280 struct
 * @returns nothing.
 */
void ReadAltitude(BMP280Handle *baro)
{
  float altitude = 0;
  float pressure = baro->pressure;
  altitude = PRESSURE_COEFFICENT * (1.0f - pow((pressure / SEA_LEVEL_PRESSURE), (1 / 5.255)));
  if ((altitude <= MIN_ALTITUDE) || altitude >= MAX_ALTITUDE)
  {
    return;
  }
  baro->altitude = altitude;
}

/*
 *  @brief  Resets the BMP280 via soft reset
 *  @retval none
 */
void ResetBMP280()
{
  Write8Bit(BMP280_REG_RESET, BMP280_RESET_VALUE);
}
