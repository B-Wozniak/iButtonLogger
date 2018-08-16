/*
 * numtoascii.c
 *
 *  Created on: 25 lip 2018
 *      Author: 710_080370
 */

#include "iButtonLogger.h"

char * BytesToOctetString(uint8_t * byte, char * buff, uint8_t size)
{
  uint8_t i;
  uint8_t high_byte, low_byte;

  for (i = 0; i < size; i++)
  {
    high_byte = (byte[i] & 0xF0) >> 4;
    low_byte =  byte[i] & 0x0F;

    buff[i] = high_byte > 9 ? 'A' + high_byte - 10 : '0' + high_byte;
    buff[i+1] = low_byte > 9 ? 'A' + low_byte - 10 : '0' + low_byte;
  }

  buff[i+1] = '\0';

  return buff;
}
