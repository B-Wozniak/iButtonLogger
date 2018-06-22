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

  /* configure used timers */
  TIM2_config();

  ConfigureSerialPorts();

  while (1);
}

