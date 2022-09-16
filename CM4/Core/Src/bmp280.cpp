#include <stm32h7xx_hal.h>
#include <string.h>
#include <spi.h>


#include <type_traits>


#include "helpers.hpp"
#include <bmp280.hpp>

Bmp280::Bmp280()
{

  bool is_config_ok = Config();
  if(is_config_ok)
  {
      ledcontrol::LEDAOn();
  }

}

void Bmp280::Run()
{
  ReadTemp();
  ReadPressure();
  ReadAltitude();
}

/*
 * @brief Pulls the NCS pin of the BMP280 High
 * @returns nothing
 */
void Bmp280::SetNSS(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

/*
 * @brief Pulls the NCS pin of the BMP280 Low
 * @returns nothing
 */
void Bmp280::ResetNSS(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}


/*
 * @brief Checks the WHOAMI register
 * to confirm the chip
 * @returns true on success, false on failure
 */
bool Bmp280::CheckID()
{
  if(Read8Bit(address::REG_ID) != address::CHIP_ID)
  {
    return false;
  }

  return true;
}

/*
 * @brief Initialises BMP280 Sensor
 * @retval true if initialisation is successful
 */
bool Bmp280::Config()
{
  ResetNSS();
  if(CheckID() != true)
  {
    return false;
  }

  ResetNSS();
  HAL_Delay(200);
  SetConfig();
  ReadCoefficients();

  return true;
}

float Bmp280::GetAltitude()
{
	return altitude;
}

float Bmp280::GetPressure()
{
	return pressure;

}

float Bmp280::GetTemperature()
{

	return temperature;

}


/**
 * @brief Reads 8 bit register.
 * @param The register adddress to be read.
 * @return 8 bit data from the register.
 */
uint8_t Bmp280::Read8Bit(const uint8_t &Register)
{
  SerialData[0] = Register | address::SPI_READ;

  ResetNSS();

  HAL_SPI_Transmit(&hspi1, &SerialData[0], 1, 10);
  HAL_SPI_Receive(&hspi1, &SerialData[1], 1, 10);
  SetNSS();
  return SerialData[1];

}
/**
 * @brief Reads 2 consecutive 8 bit register.
 * @param Register The starting address of register to be read.
 * @return the 16 bit obtained after reading 2 registers.
 */
uint16_t Bmp280::Read16Bit(const uint8_t &Register)
{
  uint16_t Out, msb, lsb;
  SerialData[0] = Register | address::SPI_READ;

  ResetNSS();
  HAL_SPI_Transmit(&hspi1, &SerialData[0], 1, 10);
  HAL_SPI_Receive(&hspi1, &SerialData[0], 2, 10);
  SetNSS();
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
uint32_t Bmp280::Read24Bit(const uint8_t &Register)
{
  uint32_t Out, msb, lsb, xlsb;
  SerialData[0] = Register | address::SPI_READ;

  ResetNSS();
  HAL_SPI_Transmit(&hspi1, &SerialData[0], 1, 10);
  HAL_SPI_Receive(&hspi1, &SerialData[0], 3, 10);
  SetNSS();
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
void  Bmp280::Write8Bit(const uint8_t &Register, const uint8_t &Data)
{
  SerialData[0] = Register & address::SPI_WRITE;
  SerialData[1] = Data;
  ResetNSS();
  HAL_SPI_Transmit(&hspi1, &SerialData[0], 2, 30);
  SetNSS();
}

uint16_t  Bmp280::Read16BitLE(const uint8_t& reg)
{
  uint16_t temp = Read16Bit(reg);
  uint16_t  retval = static_cast<uint16_t>((temp >> 8) | (temp << 8));
  return retval;
}


/*
 *  @brief  Reads the factory-set coefficients
 */
void Bmp280::ReadCoefficients()
{
 comp_values.dig_T1 = Read16BitLE(address::REG_DIG_T1);
 comp_values.dig_T2 = Read16BitLE(address::REG_DIG_T2);
 comp_values.dig_T3 = Read16BitLE(address::REG_DIG_T3);
 comp_values.dig_P1 = Read16BitLE(address::REG_DIG_P1);
 comp_values.dig_P2 = Read16BitLE(address::REG_DIG_P2);
 comp_values.dig_P3 = Read16BitLE(address::REG_DIG_P3);
 comp_values.dig_P4 = Read16BitLE(address::REG_DIG_P4);
 comp_values.dig_P5 = Read16BitLE(address::REG_DIG_P5);
 comp_values.dig_P6 = Read16BitLE(address::REG_DIG_P6);
 comp_values.dig_P7 = Read16BitLE(address::REG_DIG_P7);
 comp_values.dig_P8 = Read16BitLE(address::REG_DIG_P8);
 comp_values.dig_P9 = Read16BitLE(address::REG_DIG_P9);
}




/**
 * @brief Sets the sampling rate for both temperature and
 * pressure measurement. Standby Mode and IIR Filter can also be set set by writing
 * appropriate value to the register.The function should be called
 * in order to start measurement of both temp and pressure.
 * @param baro a pointer to the BMP280 struct
 * @returns nothing
 */
void Bmp280::SetConfig()
{
  config.mode = settings::Mode::NORMAL;
  config.pressure_oversampling = settings::Oversampling::X16;
  config.temp_oversampling = settings::Oversampling::X2;

  uint8_t temp_oversampling = to_underlying(settings::Oversampling::X2);
  uint8_t pres_oversampling = to_underlying(settings::Oversampling::X16);
  uint8_t mode = to_underlying(settings::Mode::NORMAL);
  uint8_t oversampling_data  =  (uint8_t) (temp_oversampling << 5 | pres_oversampling << 2  |mode);

  Write8Bit(address::REG_CTRL_MEAS, oversampling_data);


  config.iir_filter = settings::Filter::FILTER_16;
  config.standby = settings::StandbyTime::STANDBY_0_5;
  uint8_t filter_config;
  uint8_t filter_standby = to_underlying(settings::StandbyTime::STANDBY_0_5);
  uint8_t filter_on = to_underlying(settings::Filter::FILTER_16);
  filter_config = (uint8_t)(filter_standby << 5 | filter_on << 2);

  Write8Bit(address::REG_CONFIG, filter_config);

}


/*
 * @brief calculates the temperature reported by
 * the BMP280
 * @param baro a pointer to the BMP280 struct
 * @returns nothing
 */
void Bmp280::ReadTemp()
{
  int32_t var1, var2;
  int32_t adc_T = Read24Bit(address::REG_TEMP_MSB);
  adc_T >>= 4;

  var1 = ((((adc_T >> 3) - ((int32_t) comp_values.dig_T1 << 1))) * ((int32_t) comp_values.dig_T2)) >> 11;

  var2 = (((((adc_T >> 4) - ((int32_t) comp_values.dig_T1)) * ((adc_T >> 4) - ((int32_t) comp_values.dig_T1)))
      >> 12) * ((int32_t) comp_values.dig_T3)) >> 14;

  t_fine = var1 + var2;
  float T = (t_fine * 5 + 128) >> 8;
  temperature = (T / 100);
}

/*
 * @brief calculates the pressure reported by
 * the BMP280
 * @param baro a pointer to the BMP280 struct
 * @returns nothing
 */
void Bmp280::ReadPressure()
{

  int64_t var1, var2, p;

  // return value of t_fine
  ReadTemp();

  int32_t adc_P = Read24Bit(address::REG_PRESS_MSB);
  adc_P >>= 4;

  var1 = ((int64_t) t_fine) - 128000;
  var2 = var1 * var1 * (int64_t) comp_values.dig_P6;
  var2 = var2 + ((var1 * (int64_t) comp_values.dig_P5) << 17);
  var2 = var2 + (((int64_t) comp_values.dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t) comp_values.dig_P3) >> 8) + ((var1 * (int64_t) comp_values.dig_P2) << 12);
  var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) comp_values.dig_P1) >> 33;

  if (var1 == 0)
  {
    return; // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t) comp_values.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t) comp_values.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t) comp_values.dig_P7) << 4);

  pressure = (float) p / 256;

}

/*
 * @brief Calculates the altitude in metres above
 * sea level.
 * @param baro a pointer to the BMP280 struct
 * @returns nothing.
 */
void Bmp280::ReadAltitude()
{
  altitude = PRESSURE_COEFFICENT * (1.0f - pow((pressure / SEA_LEVEL_PRESSURE), (1 / 5.255)));
  if ((altitude <= MIN_ALTITUDE) || altitude >= MAX_ALTITUDE)
  {
    return;
  }
}

/*
 *  @brief  Resets the BMP280 via soft reset
 *  @retval none
 */
void Bmp280::Reset()
{
  Write8Bit(address::REG_RESET, address::RESET_VALUE);
}




