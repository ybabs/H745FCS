#include "ublox.h"

GPS gps;
struct NAV_PVT pvt;

void calcChecksum(unsigned char* CK)
{
	memset(CK, 0, 2);
	for (int i = 0; i < (int)sizeof(pvt); i++)
	{
		CK[0] = CK[0] + ((unsigned char*)(&pvt))[i];
		CK[1] = CK[1] + CK[0];
	}

}

void processGPS()
{
		gps.latitude = pvt.latitude * 1.0e-7;
		gps.longitude = pvt.longitude * 1.0e-7;
		gps.altitude = pvt.height / 1000.0f;

		// format to ENU
		gps.vel_x = pvt.velE / 1000.0f;
		gps.vel_y = pvt.velN / 1000.0f;
		gps.vel_z = pvt.velD / -1000.0f;
		gps.gndSpeed = pvt.gSpeed / 1000.0f;
}
