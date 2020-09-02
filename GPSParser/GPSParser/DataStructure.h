//#include <stdio.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <string.h>
//
//#define USE_GGA
//#define USE_RMC
//
//typedef enum
//{
//	UNDEFINED,
//	GGA,
//	RMC
//}NMEASentence;
//
//
//typedef struct RMCData
//{
//	char* UTC;
//	uint8_t status;
//	float Latitude;
//	char Lat_Direction;
//	float Longitude;
//	char Lon_Direction;
//	float ground_speed;
//	float track_heading;
//	uint8_t date;
//	float mag_variation;
//	char Mag_Dir;
//	uint8_t checksum;
//}RMCData;
//
//typedef struct GGAData
//{
//	char* UTC;
//	float Latitude;
//	char* Lat_Direction;
//	float Longitude;
//	char* Lon_Direction;
//	uint8_t gps_quality;
//	uint8_t num_satellites;
//	float hdop;
//	float altitude;
//	char alt_unit;
//	float msl_altitude;
//	char msl_unit;
//	uint8_t checksum;
//
//}GGAData;
//
//
//typedef struct GPSData
//{
//	uint8_t UTC;
//	float latitude_dec;
//	float longitude_dec;
//	char latitude_dir;
//	char longitude_dir;
//	uint8_t fix_quality;
//	uint8_t num_satellites;
//	float hdop;
//	float altitude;
//	float mean_sea_level;
//	uint8_t fix_status;
//	float ground_speed;
//	float magnetic_variation;
//
//	NMEASentence sentence;
//	char header_buf[6];
//
//
//}GPSData;