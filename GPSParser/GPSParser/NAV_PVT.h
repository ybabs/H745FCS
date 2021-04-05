

typedef struct NAV_PVT {
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
}NAV_PVT;

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

void calcChecksum(unsigned char* CK);
extern void processGPS(); // change to HAL_StatusTypeDef
