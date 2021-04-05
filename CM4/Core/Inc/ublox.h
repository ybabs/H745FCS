#ifndef UBLOX_H_
#define UBLOX_H_
#include "string.h"

#define UBLOX_GXGGA_OFF (const char []) {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24}
#define UBLOX_GXGLL_OFF (const char[])  {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B}
#define UBLOX_GXGSA_OFF (const char[]) {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x32}
#define UBLOX_GXGSV_OFF  (const char []) {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x39}
#define UBLOX_GXRMC_OFF (const char []) {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x40}
#define UBLOX_GXVTG_OFF (const char []) {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x47}
#define UBLOX_10HZ_RATE (const char []) {0xB5,0x62,0x06,0x08,0x06,0x00,0x64,0x00,0x01,0x00,0x01,0x00,0x7A,0x12}
#define UBLOX_NAVPVT_ON (const char []) {0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x18,0xE1}
#define UBX_HEADER (const char []){ 0xB5, 0x62 }

  struct NAV_PVT {
 	unsigned char cls;
 	unsigned char id;
 	unsigned char length;
 	unsigned long iTOW; // GPS Time of Week in ms

 	unsigned short year; // UTC Year
 	unsigned char month; // month in UTC
 	unsigned char day;
 	unsigned char hour;
 	unsigned char min;
 	unsigned char sec;
 	unsigned char validflag;
 	unsigned long tAcc; // time accuracy estimate in  ns
 	long nano;  // ns fraction of second
 	unsigned char fixType; // GNSS fix type: {0: no Fix, 1 dead reckoning, 2, 2D-fix, 3:3D fix, 4: GNSS + dead reckoning, 5: time fix
 	unsigned char fixStatusflag;
 	unsigned char extendedflag;
 	unsigned char numSV; // number of satellites used
 	long longitude;
 	long latitude;
 	long height; // height above ellipsoid
 	long hMSL; // height above mean sea level
 	unsigned long hAcc; // // horizontal accuracy estimate
 	unsigned long vAcc; // vertical accuracy estimate

     long velN;
 	long velE;
 	long velD;
     long gSpeed; // Ground Speed
     long headMot; // 2D head motion
 	unsigned long sAcc; // speed acuracy motion
 	unsigned long headingAcc; // heading accuracy estimate

 	unsigned short pDOP;
 	unsigned char flags3;

 	unsigned char reserved1;
 	unsigned char reserved2;
 	unsigned char reserved3;
 	unsigned char reserved4;
 	unsigned char reserved5;

 	long headVeh; // heading Velocity
     short magDec;
     short magAcc;
 };

 typedef struct GPS
 {
 	float latitude;
 	float longitude;
 	float altitude;  // Based on height above ellipsoid. h = N + H, h = height, H = hMSL
 	float vel_x;
 	float vel_y;
 	float vel_z;
 	float gndSpeed;

 }GPS;

 extern struct NAV_PVT pvt;
 void calcChecksum(unsigned char* CK);
 void processGPS(); // change to HAL_StatusTypeDef

#endif /* UBLOX_H_*/

