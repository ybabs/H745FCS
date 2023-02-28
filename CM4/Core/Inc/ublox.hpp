#pragma once

#include <cstring>
#include <stdint.h>
#include <array>
#include <helpers.hpp>

static constexpr char UBLOX_GXGGA_OFF[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24};
static constexpr char UBLOX_GXGLL_OFF[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B};
static constexpr char  UBLOX_GXGSA_OFF[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x32};
static constexpr char  UBLOX_GXGSV_OFF[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x39};
static constexpr char  UBLOX_GXRMC_OFF[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x40};
static constexpr char  UBLOX_GXVTG_OFF[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x47};
static constexpr char  UBLOX_10HZ_RATE[] = {0xB5,0x62,0x06,0x08,0x06,0x00,0x64,0x00,0x01,0x00,0x01,0x00,0x7A,0x12};
static constexpr char  UBLOX_NAVPVT_ON[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x18,0xE1};
static constexpr char  UBX_HEADER[]      = {0xB5, 0x62 };


template<typename T>
T *get_temp_addr(T &&x) {
    return &x;
}

struct NAV_PVT
{
  unsigned char cls;
  unsigned char id;
  unsigned char length;
  unsigned long iTOW;               // GPS Time of Week in ms
  unsigned short year;             // UTC Year
  unsigned char month;            // month in UTC
  unsigned char day;
  unsigned char hour;
  unsigned char min;
  unsigned char sec;
  unsigned char validflag;
  unsigned long tAcc;                        // time accuracy estimate in  ns
  long nano;                                // ns fraction of second
  unsigned char fixType;                   // GNSS fix type: {0: no Fix, 1 dead reckoning, 2, 2D-fix, 3:3D fix, 4: GNSS + dead reckoning, 5: time fix
  unsigned char fixStatusflag;
  unsigned char extendedflag;
  unsigned char numSV;                   // number of satellites used
  long longitude;
  long latitude;
  long height;                         // height above ellipsoid
  long hMSL;                          // height above mean sea level in mm
  unsigned long hAcc;                // horizontal accuracy estimate
  unsigned long vAcc;               // vertical accuracy estimate
  long velN;
  long velE;
  long velD;
  long gSpeed;                    // Ground Speed
  long headMot;                  // 2D head motion
  unsigned long sAcc;           // speed accuracy motion
  unsigned long headingAcc;    // heading accuracy estimate
  unsigned short pDOP;
  unsigned char flags3;
  unsigned char reserved1;
  unsigned char reserved2;
  unsigned char reserved3;
  unsigned char reserved4;
  unsigned char reserved5;
  long headVeh;               // heading Velocity
  short magDec;
  short magAcc;
};

void calcChecksum(unsigned char* ck);
void ProcessUART();



class GPS{



public:
  GPS();
  gpsData GetGPSData();
  //static void CalcChecksum(unsigned char* ck);
  static NAV_PVT GetPVTStruct();
  static int GetDataSize();
  void ProcessGPS();





private:
  float latitude;
  float longitude;
  float altitude;  // Based on height above ellipsoid. h = N + H, h = height, H = hMSL
  float vel_x;
  float vel_y;
  float vel_z;
  float gndSpeed;
  uint8_t num_satellites;
  short magnetic_declination;
  unsigned long time_of_week;
//  static const int data_size = sizeof(NAV_PVT);
  void Config();




};
