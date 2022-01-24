/*
 * sbus.c
 *
 *  Created on: Nov 6, 2021
 *      Author: Daniel Babatunde
 */

#include "sbus.h"

SBUS_CH sbus_channel;

/*
 * @Note Annoying quirk with this build is sbus_header is always in the first byte captured
 * This is not seen in standalone builds testing SBUS only. NOt sure why this is happening.
 * As a result, whe're checking for the header in the first byte and shifting everything
 * by one byte.
 */
void updateSbus(uint8_t *buf)
{
  if (buf[1] == SBUS_HEADER)
  {
    sbus_channel.channel1 = (buf[2] >> 0 | (buf[3] << 8)) & 0x07FF;
    sbus_channel.channel2 = (buf[3] >> 3 | (buf[4] << 5)) & 0x07FF;
    sbus_channel.channel3 = (buf[4] >> 6 | (buf[5] << 2) | buf[6] << 10) & 0x07FF;
    sbus_channel.channel4 = (buf[6] >> 1 | (buf[7] << 7)) & 0x07FF;
    sbus_channel.channel5 = (buf[7] >> 4 | (buf[8] << 4)) & 0x07FF;
    sbus_channel.channel6 = (buf[8] >> 7 | (buf[9] << 1) | buf[10] << 9) & 0x07FF;
    sbus_channel.channel7 = (buf[10] >> 2 | (buf[11] << 6)) & 0x07FF;
    sbus_channel.channel8 = (buf[11] >> 5 | (buf[12] << 3)) & 0x07FF;
    sbus_channel.channel9 = (buf[13] << 0 | (buf[14] << 8)) & 0x07FF;
    sbus_channel.channel10 = (buf[14] >> 3 | (buf[15] << 5)) & 0x07FF;
    sbus_channel.channel11 = (buf[15] >> 6 | (buf[16] << 2) | buf[16] << 10) & 0x07FF;
    sbus_channel.channel12 = (buf[17] >> 1 | (buf[18] << 7)) & 0x07FF;
    sbus_channel.channel13 = (buf[18] >> 4 | (buf[19] << 4)) & 0x07FF;
    sbus_channel.channel14 = (buf[19] >> 7 | (buf[20] << 1) | buf[20] << 9) & 0x07FF;
    sbus_channel.channel15 = (buf[21] >> 2 | (buf[22] << 6)) & 0x07FF;
    sbus_channel.channel16 = (buf[22] >> 5 | (buf[23] << 3)) & 0x07FF;

    if (buf[24] & (1 << 0))
    {
      channel_17 = 1;
    }
    else
    {
      channel_17 = 0;
    }

    if (buf[24] & (1 << 1))
    {
      channel_17 = 1;
    }
    else
    {
      channel_17 = 0;
    }
    // Failsafe
    failsafe_status = SBUS_SIGNAL_OK;
    if (buf[24] & (1 << 2))
    {
      failsafe_status = SBUS_SIGNAL_LOST;
    }

    if (buf[24] & (1 << 3))
    {
      failsafe_status = SBUS_SIGNAL_FAILSAFE;
    }
  }
}




/********************************************************* FUTURE IMPLEMENTATION******************************************/
//uint8_t sbus_state;
//uint8_t prev_byte;
//uint8_t curr_byte;
//
//uint8_t failsafe_status = 0;
//uint8_t lost_frame = 0;
//uint8_t updateSbus()
//{
//
//}
//
//uint8_t parseSbus()
//{
//
//}
