/*
 * main.c
 *
 *  Created on: 29 sie 2017
 *      Author: bwozniak
 */

#include "iButtonLogger.h"

int main(void)
{
  /* set system and bus clocks */
  SystemClockConfig();

  /* configure gpios */
  GpioConfig();

  ConfigureSerialPorts();

  /* Init OneWire */
  OWInit();

  while (1)
  {
    if (one_wire_state == button_read)
    {
      if (CheckCrc(ibutton.key_byte) == SUCCESS)
      {
        _set_high(GREEN_LED_PORT, GREEN_LED_PIN);
        SerialSendString(USART2, "iButton Key: ");
//        TODO: SerialSendString(USART2, ByteToAsciHex(*buff, ibutton.key_byte));
//        a moze SerialSendKey() ?
      }
      else
        SerialSendString(USART2, "Readout failure\n");

      OWPollingInit();
    }
  }
}

