//#include "parser.h"
////#include "strsep.h"
//
//
//
//void InitParser(GPSData* data)
//{
//	data->sentence = UNDEFINED;
//
//    
//
//	
//}
//
//
//void ExtractGGA(char* data_string)
//{
//    char* token;
//    
//    int i = 0;
//    while ((token = strsep(&data_string, "," )) != NULL)
//    {
//        switch (i)
//        {
//        case 1:
//            GGAStruct.UTC = token;
//            break;
//        case 2:
//        {
//            float value = atof(token);
//            GGAStruct.Latitude = convertToDegDecimal(value);
//            break;
//        }
//        case 3:
//        {
//            GGAStruct.Lat_Direction = token;
//            if (*token == 'S')
//            {
//                GGAStruct.Latitude = GGAStruct.Latitude * -1.0f;
//            }
//            break;
//        }
//
//        case 4:
//        {
//            float value = atof(token);
//            GGAStruct.Longitude = convertToDegDecimal(value);
//            break;
//        }
//
//        case 5:
//        {
//            GGAStruct.Lon_Direction = token;
//            if (*token == 'W')
//            {
//                GGAStruct.Longitude = GGAStruct.Longitude * -1.0f;
//            }
//        }
//        case 6:
//            break;
//        case 7:
//            break;
//        case 8:
//            break;
//        case 9:
//            break;
//        case 10:
//            break;
//        case 11:
//            break;
//        case 12:
//            break;
//        case 13:
//            break;
//        case 14:
//            break;
//
//        default:
//            break;
//        }
//       
//        i++;
//    }
//
//    ParseGPS();
//
//}
//
//float convertToDegDecimal(float deg_mins)
//{
//    float deg_whole =  (int) (deg_mins / 100);
//
//    float deg_decimal = (deg_mins - deg_whole * 100) / 60;
//
//    return  deg_whole + deg_decimal;
//}
//
//void ExtractRMC(char* data_string)
//{
//
//}
//
////void ParseGPS(GPSData* data)
////{
////
////	char* found;
////	while (found = strsep(&sample_RMC, delim) != NULL)
////	{
////		printf("%s\n", found);
////	}
////
////
////}
//
//
//
//void ParseGPS()
//{
//
//	//char* found;
// //  
// //   char* ptr = sample_string;
//	//while (found = strsep(&ptr, ",") != NULL)
//	//{
//	//	printf("%s\n", found);
//	//}
//   //char * ptr = sample_GGA, * found;
//    //char buffer[200];
//
//    //printf("Original string: %s\n", sample_GGA);
//    //while ((found = strsep(&ptr, ",")) != NULL) {
//    //    printf("got token [%s]\n", found);
//    //}
//
//
//}
//
//char* strsep(char** stringp, const char* delim)
//{
//    char* start = *stringp, * p = start ? strpbrk(start, delim) : NULL;
//
//    if (!p) {
//        *stringp = NULL;
//    }
//    else {
//        *p = 0;
//        *stringp = p + 1;
//    }
//
//    return start;
//}