/*
 * timers.c
 *
 *  Created on: 8 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

void TIM2_config(void)
{
  TIM2->PSC = 4000;
  TIM2->ARR = 1000;
  TIM2->DIER |= TIM_DIER_UIE; /* interrupt enable on 1st channel */
  TIM2->EGR = 1; /* event generation (to load all tim2 settings) */
  TIM2->SR = 0;
  NVIC_EnableIRQ(TIM2_IRQn);
  TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void)
{
  TIM2->SR = 0;
  GPIOE->ODR ^= (1 << 8);
}