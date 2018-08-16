/*
 * main.c
 *
 *  Created on: 29 sie 2017
 *      Author: bwozniak
 */

#include "iButtonLogger.h"

char buff[50];
char * buff_ptr = buff;

int main(void)
{
  /* set system and bus clocks */
  SystemClockConfig();

  /* configure gpios */
  GpioConfig();

  /* interfaces */
  SerialInit();
  OneWireInit();

  OneWirePoll();
  while (1)
  {
    if (ow_flags & OW_BUTTON_READ_SUCCESS)
    {
      ow_flags = 0;
      SerialSendString(USART2, BytesToOctetString(ibutton.key_byte, buff, KEY_SIZE_BYTE));
    }
  }
}

