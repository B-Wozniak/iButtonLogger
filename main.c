/*
 * main.c
 *
 *  Created on: 29 sie 2017
 *      Author: bwozniak
 */

#include "iButtonLogger.h"

/* ********************** DEBUG ******************** */
//volatile uint8_t state;
//volatile uint8_t prev_state = 0;

//typedef struct
//{
//  EOwState one_wire_state;
//  uint32_t time[64];
//  uint32_t bus_state[64];
//}TOWdebug;
//
//TOWdebug ow_logger;
//TOWdebug * logger = &ow_logger;
/* ********************** DEBUG ******************** */

int main(void)
{
  /* set system and bus clocks */
  SystemClockConfig();

  /* configure gpios */
  GpioConfig();
//  gpio_pin_cfg(GPIOH, 0, GPIO_IN_FLOATING);

  /* Init OneWire */
  OWInit();
//  TIM5_config();
//  TIM2_config();

//  ConfigureSerialPorts();
//  TIM5->CR1 |= TIM_CR1_CEN;
  while (1)
  {
//    if (one_wire_state == write)
//    {
//      state = _gpio_read(GPIOH, 0);
//      if (state != prev_state)
//      {
//        logger->time[i++] = TIM5->CNT;
//        logger->bus_state[i++] = state;
//        prev_state = state;
//      }
//    }
  }
}

