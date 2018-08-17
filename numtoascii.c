/*
 * numtoascii.c
 *
 *  Created on: 25 lip 2018
 *      Author: 710_080370
 */

#include "iButtonLogger.h"

uint8_t * BytesToOctetString(uint8_t * byte, uint8_t * buff, uint8_t size)
{
  uint8_t i = 0, j = 0;
  uint8_t high_byte, low_byte;

  if (size < 1)
    return 0;

  while (i < size)
  {
    high_byte = (byte[i] & 0xF0) >> 4;
    low_byte  = (byte[i] & 0x0F);

    buff[j++] = (uint8_t)(high_byte > 9 ? 'A' + high_byte - 10 : '0' + high_byte);
    buff[j++] = (uint8_t)(low_byte > 9 ? 'A' + low_byte - 10 : '0' + low_byte);
    i++;
  }

  buff[j] = '\0';

  return buff;
}

uint8_t * ByteToAsciiHex(uint8_t * dest, uint8_t byte)
{
  uint8_t temp;

  /* most significant byte */
  temp = (byte & 0xF0) >> 4;
  *dest++ = (uint8_t)(temp > 9 ? 'A' + temp - 10 : '0' + temp);

  /* least significant byte */
  temp = byte & 0x0F;
  *dest++ = (uint8_t)(temp > 9 ? 'A' + temp - 10 : '0' + temp);

  return dest;
}
