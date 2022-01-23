///*
// * sbus.c
// *
// *  Created on: Nov 6, 2021
// *      Author: Daniel Babatunde
// */
//
//#include "sbus.h"
//
//uint8_t updateSbus(uint8_t *buf)
//{
//
//  if (buf[23] == 0)
//  {
//    sbus_channel.channel1 = ((int16_t) buf[1] >> 0 | ((int16_t) buf[2] << 8)) & 0x07FF;
//    sbus_channel.channel2 = ((int16_t) buf[2] >> 3 | ((int16_t) buf[3] << 5)) & 0x07FF;
//    sbus_channel.channel3 = ((int16_t) buf[3] >> 6 | ((int16_t) buf[4] << 2) | (int16_t) buf[5] << 10) & 0x07FF;
//    sbus_channel.channel4 = ((int16_t) buf[5] >> 1 | ((int16_t) buf[6] << 7)) & 0x07FF;
//    sbus_channel.channel5 = ((int16_t) buf[6] >> 4 | ((int16_t) buf[7] << 4)) & 0x07FF;
//    sbus_channel.channel6 = ((int16_t) buf[7] >> 7 | ((int16_t) buf[8] << 1) | (int16_t) buf[9] << 9) & 0x07FF;
//    sbus_channel.channel7 = ((int16_t) buf[9] >> 2 | ((int16_t) buf[10] << 6)) & 0x07FF;
//    sbus_channel.channel8 = ((int16_t) buf[10] >> 5 | ((int16_t) buf[11] << 3)) & 0x07FF;
//
//    sbus_channel.channel9 = ((int16_t) buf[12] >> 0 | ((int16_t) buf[13] << 8)) & 0x07FF;
//    sbus_channel.channel10 = ((int16_t) buf[13] >> 3 | ((int16_t) buf[14] << 5)) & 0x07FF;
//    sbus_channel.channel11 = ((int16_t) buf[14] >> 6 | ((int16_t) buf[15] << 2) | (int16_t) buf[16] << 10) & 0x07FF;
//    sbus_channel.channel12 = ((int16_t) buf[16] >> 1 | ((int16_t) buf[17] << 7)) & 0x07FF;
//    sbus_channel.channel13 = ((int16_t) buf[17] >> 4 | ((int16_t) buf[18] << 4)) & 0x07FF;
//    sbus_channel.channel14 = ((int16_t) buf[18] >> 7 | ((int16_t) buf[19] << 1) | (int16_t) buf[20] << 9) & 0x07FF;
//    sbus_channel.channel15 = ((int16_t) buf[20] >> 2 | ((int16_t) buf[21] << 6)) & 0x07FF;
//    sbus_channel.channel16 = ((int16_t) buf[21] >> 5 | ((int16_t) buf[22] << 3)) & 0x07FF;
//
//    return 1;
//  }
//
//  else
//  {
//    sbus_channel.connect_state = 0;
//    return 0;
//  }
//
//
//
//}
//
