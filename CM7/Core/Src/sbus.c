/*
 * sbus.c
 *
 *  Created on: Nov 6, 2021
 *      Author: Daniel Babatunde
 */

#include "sbus.h"

SBUS_CH sbus_channel;
uint16_t failsafe_status;
uint16_t channel_17;

void updateSbus(uint8_t *buf)
{
    sbus_channel.channel1 = (buf[1] >> 0 | (buf[2] << 8)) & 0x07FF;
    sbus_channel.channel2 = (buf[2] >> 3 | (buf[3] << 5)) & 0x07FF;
    sbus_channel.channel3 = (buf[3] >> 6 | (buf[4] << 2) | buf[5] << 10) & 0x07FF;
    sbus_channel.channel4 = (buf[5] >> 1 | (buf[6] << 7)) & 0x07FF;
    sbus_channel.channel5 = (buf[6] >> 4 | (buf[7] << 4)) & 0x07FF;
    sbus_channel.channel6 = (buf[7] >> 7 | (buf[8] << 1) | buf[9] << 9) & 0x07FF;
    sbus_channel.channel7 = (buf[9] >> 2 | (buf[10] << 6)) & 0x07FF;
    sbus_channel.channel8 = (buf[10] >> 5 | (buf[11] << 3)) & 0x07FF;
    sbus_channel.channel9 = (buf[12] << 0 | (buf[13] << 8)) & 0x07FF;
    sbus_channel.channel10 = (buf[13] >> 3 | (buf[14] << 5)) & 0x07FF;
    sbus_channel.channel11 = (buf[14] >> 6 | (buf[15] << 2) | buf[16] << 10) & 0x07FF;
    sbus_channel.channel12 = (buf[16] >> 1 | (buf[17] << 7)) & 0x07FF;
    sbus_channel.channel13 = (buf[17] >> 4 | (buf[18] << 4)) & 0x07FF;
    sbus_channel.channel14 = (buf[18] >> 7 | (buf[19] << 1) | buf[20] << 9) & 0x07FF;
    sbus_channel.channel15 = (buf[20] >> 2 | (buf[21] << 6)) & 0x07FF;
    sbus_channel.channel16 = (buf[21] >> 5 | (buf[22] << 3)) & 0x07FF;

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
