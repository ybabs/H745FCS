#include "bmp280.h"


uint8_t CheckChipID()
{

  volatile uint8_t SerialData[3] = {(BMP280_REG_ID | 0x80), 0, 0};
  volatile uint8_t aRxBuffer[3]= {0};
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
  if(HAL_SPI_TransmitReceive_DMA(&hspi1, SerialData,  (uint8_t*)aRxBuffer, 2) == HAL_OK)
  {
    if(aRxBuffer[1] == BMP280_CHIP_ID)
    {
      return HAL_OK;
    }

  }

  else
  {
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
    return HAL_ERROR;

  }

}


uint8_t Read8Bit(BMP280Handle* baro)
{
  return HAL_OK;
}
uint16_t Read16Bit(uint8_t reg)
{
   volatile uint16_t result;
   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
   uint8_t SerialData[3] = {(reg|0x80), 0, 0};
   volatile uint8_t aRxBuffer[3]= {0};
   HAL_SPI_TransmitReceive_DMA(&hspi1, SerialData,  (uint8_t*)aRxBuffer, 3);

   result = aRxBuffer[2] << 8 | aRxBuffer [1];
   return result;
}


uint32_t Read24Bit(uint8_t reg)
{
  uint32_t result;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  uint8_t SerialData[4] = {(reg|0x80), 0, 0,0};
  volatile uint8_t aRxBuffer[4] = {0};
  HAL_SPI_TransmitReceive_DMA(&hspi1, SerialData,  (uint8_t*)aRxBuffer, 4);
  result = aRxBuffer[1] << 16 | aRxBuffer[2] << 8 | aRxBuffer[3] >> 4;
  return result;
}

void ReadTemp(BMP280Handle* baro)
{
  int32_t var1, var2;

  int32_t adc_T = Read24Bit(BMP280_REG_TEMP_MSB);
  adc_T >>= 4;

  var1 = ((((adc_T>> 3) - ((int32_t)baro->comp_vals.dig_T1 <<1)))*
         ((int32_t)baro->comp_vals.dig_T2)) >> 11;

  var2 = (((((adc_T >> 4) - ((int32_t)baro->comp_vals.dig_T1)) *
             ((adc_T >> 4) - ((int32_t)baro->comp_vals.dig_T1))) >>
            12) *
           ((int32_t)baro->comp_vals.dig_T3)) >>
          14;

  t_fine = var1 + var2;

  float T = (t_fine * 5 + 128) >> 8;
  baro->temperature = T/100;

}

void ReadPressure(BMP280Handle* baro)
{

  int64_t var1, var2, p;

  // return value of t_fine
  ReadTemp(baro);

  int32_t adc_P = Read24Bit(BMP280_REG_PRESS_MSB);
  adc_P >>=4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)baro->comp_vals.dig_P6;
  var2 = var2 + ((var1 * (int64_t)baro->comp_vals.dig_P5) << 17);
  var2 = var2 + (((int64_t)baro->comp_vals.dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)baro->comp_vals.dig_P3) >> 8) +
           ((var1 * (int64_t)baro->comp_vals.dig_P2) << 12);
  var1 =
        (((((int64_t)1) << 47) + var1)) * ((int64_t)baro->comp_vals.dig_P1) >> 33;

    if (var1 == 0) {
        return; // avoid exception caused by division by zero
      }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)baro->comp_vals.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)baro->comp_vals.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)baro->comp_vals.dig_P7) << 4);

    baro->pressure = (float) p/256;

}




void ReadCalibCoefficients(BMP280Handle* baro)
{
  baro->comp_vals.dig_T1 = Read16Bit(dig_T1);
  baro->comp_vals.dig_T2 = Read16Bit(dig_T2);
  baro->comp_vals.dig_T3 = Read16Bit(dig_T3);
  baro->comp_vals.dig_P1 = Read16Bit(dig_P1);
  baro->comp_vals.dig_P2 = Read16Bit(dig_P2);
  baro->comp_vals.dig_P3 = Read16Bit(dig_P3);
  baro->comp_vals.dig_P4 = Read16Bit(dig_P4);
  baro->comp_vals.dig_P5 = Read16Bit(dig_P5);
  baro->comp_vals.dig_P6 = Read16Bit(dig_P6);
  baro->comp_vals.dig_P7 = Read16Bit(dig_P7);
  baro->comp_vals.dig_P8 = Read16Bit(dig_P8);
  baro->comp_vals.dig_P9 = Read16Bit(dig_P9);

}
void WriteRegister(uint8_t reg, uint8_t data)
{

}
uint8_t setConfig(BMP280Handle* baro)
{

   baro->config.mode = NORMAL;
   baro->config.pressure_oversampling = X2;
   baro->config.temp_oversampling = X2;

   uint8_t SerialData[2] = {0};
   volatile uint8_t aRxBuffer[3]= {0};
   SerialData[0] = BMP280_REG_CTRL_MEAS & ~0x80;
   SerialData[1] = baro->config.temp_oversampling | baro->config.pressure_oversampling | baro->config.mode ;
   SerialData[1] = 0x27;

   if(HAL_SPI_TransmitReceive_DMA(&hspi1, SerialData, (uint8_t*)aRxBuffer, 3) == HAL_OK)
   {
     return HAL_OK;
   }

   else
   {
     return HAL_ERROR;
   }


}




void  HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI1)
    {

      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
      //ReadComplete();

    }


}


void ReadComplete()
{


  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);

}

uint8_t ResetChip(void)
{
   uint8_t SerialData[2] = {(BMP280_REG_RESET & ~0x80), BMP280_RESET_VALUE}; // Register address, Data,
   volatile uint8_t aRxBuffer[3]= {0};
   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);

   if(HAL_SPI_TransmitReceive_DMA(&hspi1, SerialData, (uint8_t*)aRxBuffer, 2) == HAL_OK)
   {
     return HAL_OK;
   }

   else
   {
     return HAL_ERROR;
   }



}
