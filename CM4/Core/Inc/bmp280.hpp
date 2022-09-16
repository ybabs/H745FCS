/*
 * bmp280.hpp
 *
 *  Created on: Aug 27, 2022
 *      Author: Daniel Babatunde
 */

#pragma once


#include <stdint.h>
#include <array>

#include "bmp_280_registers.hpp"


static constexpr float SEA_LEVEL_PRESSURE   =  101325;  // in Pa from 1013.25hPa
static constexpr float PRESSURE_COEFFICENT  = 44330.0;
static constexpr float MAX_ALTITUDE        = 9000; // in metres above sea-level
static constexpr float MIN_ALTITUDE        = -500;    // in metres


class Bmp280
{
   struct  CompensationValues
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
    };

   struct __attribute__((__packed__))Config
   {
     settings::Mode  mode;
     settings::Filter iir_filter;
     settings::StandbyTime standby;
     settings::Oversampling pressure_oversampling;
     settings::Oversampling temp_oversampling;
   };

public:
   Bmp280();
   float GetPressure();
   float GetAltitude();
   float GetTemperature();
   void Run();



private:
   int32_t t_fine;
   float temperature;
   float pressure;
   double altitude;
   std::array<uint8_t, 3>SerialData;
   Bmp280::Config config;
   Bmp280::CompensationValues comp_values;

   bool Config();
   void Reset();
   bool CheckID();
   uint8_t Read8Bit(const uint8_t &Register);
   uint16_t Read16Bit(const uint8_t &Register);
   uint32_t Read24Bit(const uint8_t &Register);
   void Write8Bit(const uint8_t &Register, const uint8_t &Data);
   uint16_t Read16BitLE(const uint8_t& Register);
   void ReadCoefficients();
   void SetConfig();
   void ReadTemp();
   void ReadPressure();
   void ReadAltitude();
   void SetNSS();
   void ResetNSS();


   template <typename E>
   constexpr auto to_underlying(E e) noexcept
   {
       return static_cast<std::underlying_type_t<E>>(e);
   }


};



