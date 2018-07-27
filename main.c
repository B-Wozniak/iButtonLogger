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

  /* interfaces */
  SerialInit();
  OneWireInit();

  OWPollingInit();
  while (1) continue;
}

