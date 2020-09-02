//#include <stdio.h>
//#include <string.h>
//
//#include "parser.h"
//
//char sample_GGA[] = "$GNGGA,173835.00,5246.07397,N,00113.98792,W,2,12,0.79,55.7,M,47.2,M,,0000*62";
//char sample_RMC[] = "$GNRMC,221203.00,A,5246.04986,N,00113.99992,W,0.136,,270820,,,D*75";
//
//char GGA_KEY[6] = "$GNGGA";
//char RMC_KEY[6] = "$GNRMC";
//
//GGAData gga_data;
//RMCData rmc_data;
//
//int main()
//{
//	char buffer[7] = {0};
//	int test_val = 0;
//	
//	
//	memcpy(buffer, sample_GGA, 6);
//
//	if (strcmp(buffer, GGA_KEY) == 0)
//	{
//		ExtractGGA(sample_GGA);
//		test_val = 1;
//		
//	}
//
//	else if (strcmp(buffer, RMC_KEY) == 0)
//	{
//		ExtractRMC(sample_GGA);
//		test_val = 2;
//	}
//
//	printf("Test: %d", test_val);
//
//
//	ParseGPS();
//	return 0;
//}

#include "NAV_PVT.h"

int main()
{
	processGPS();
	return 0;
}