/*
 * main.c
 *
 *  Created on: 29 sie 2017
 *      Author: bwozniak
 */

#include "iButtonLogger.h"


int main(void)
{
  // cont pointer to non-const structure

//  SystemClockConfig();

  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

  GPIOE->MODER &= ~(3 << (2 * 8));
  GPIOE->MODER |= (1 << (2 * 8));

  TIM2_config();

  while (1);
}

