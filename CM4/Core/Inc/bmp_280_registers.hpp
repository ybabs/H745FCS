/*
 * bmp_280_registers.hpp
 *
 *  Created on: Aug 27, 2022
 *      Author: Daniel Babatunde
 */

#pragma once

namespace address
{

static constexpr uint8_t CHIP_ID          = 0x58;
static constexpr uint8_t  REG_ID          = 0xD0;
static constexpr uint8_t  REG_RESET       = 0xE0;
static constexpr uint8_t  REG_STATUS      = 0xF3;
static constexpr uint8_t  REG_CTRL_MEAS   = 0xF4;
static constexpr uint8_t  REG_CONFIG      = 0xF5;
static constexpr uint8_t  REG_PRESS_MSB   = 0xF7; // 19:12
static constexpr uint8_t  REG_PRESS_LSB   = 0xF8; // 4:11
static constexpr uint8_t  REG_PRESS_XLSB  = 0xF9;  //bits 4 - 7
static constexpr uint8_t  REG_TEMP_MSB    = 0xFA;
static constexpr uint8_t  REG_TEMP_LSB    = 0xFB;
static constexpr uint8_t  REG_TEMP_XLSB   = 0xFC;
static constexpr uint8_t  RESET_VALUE     = 0xB6;   // readout value is always 0x00
static constexpr uint8_t  SPI_READ        = 0x80;
static constexpr uint8_t  SPI_WRITE       = 0X7F;

static constexpr uint16_t REG_DIG_T1 =  0x88;
static constexpr uint16_t REG_DIG_T2 =  0x8A;
static constexpr uint16_t REG_DIG_T3 =  0x8C;
static constexpr uint16_t REG_DIG_P1 =  0x8E;
static constexpr uint16_t REG_DIG_P2 =  0x90;
static constexpr uint16_t REG_DIG_P3 =  0x92;
static constexpr uint16_t REG_DIG_P4 =  0x94;
static constexpr uint16_t REG_DIG_P5 =  0x96;
static constexpr uint16_t REG_DIG_P6 =  0x98;
static constexpr uint16_t REG_DIG_P7 =  0x9A;
static constexpr uint16_t REG_DIG_P8 =  0x9C;
static constexpr uint16_t REG_DIG_P9 =  0x9E;

}

namespace settings
{

  enum class Status
  {
    IM_UPDATE = 0x01,
    MEASURING = 0x08
  };

  enum class Oversampling
  {
    SKIPPED = 0x00, // No measurement
    X1 = 0x01, // x 2
    X2 = 0x02,
    X4 = 0x03,
    X8 = 0x04,
    X16 = 0x05

  };

  enum class Mode
  {
    SLEEP = 0x00,
    FORCED = 0x01,
    NORMAL = 0x03
  };

 enum class Filter
  {
    FILTER_OFF = 0x00,
    FILTER_2 = 0x01,
    FILTER_4 = 0x02,
    FILTER_8 = 0x03,
    FILTER_16 = 0x04
  };

 enum class StandbyTime
  {
    STANDBY_0_5 = 0x00, // 0.5 ms
    STANDBY_62_5 = 0x01, //62.5ms
    STANDBY_125 = 0x02, //125 ms
    STANDBY_250 = 0x03, // 250ms
    STANDBY_500 = 0x04, // 500 ms
    STANDBY_1000 = 0x05, // 1000ms
    STANDBY_2000 = 0x06, // 2000ms
    STANDBY_4000 = 0x07  // 4000ms

  };

}

