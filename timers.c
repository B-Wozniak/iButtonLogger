/*
 * timers.c
 *
 *  Created on: 8 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

// timer for led blink purposes
void TIM2_config(void)
{
  /* turn on peripheral */
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

  /* settings */
  TIM2->PSC = (10000 * 2) - 1;
  TIM2->ARR = 1000 - 1;
  TIM2->DIER |= TIM_DIER_UIE; /* interrupt enable on 1st channel */
  NVIC_EnableIRQ(TIM2_IRQn);
  TIM2->CR1 |= TIM_CR1_CEN;
}

/* 1Hz timer */
void TIM5_config(void)
{
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
  TIM5->PSC = (80 * APB1_TIMER_MULT) - 1;
  TIM5->ARR = 1000000 - 1;
//  TIM5->CR1 |= TIM_CR1_OPM;
  TIM5->DIER = TIM_DIER_UIE;
  NVIC_EnableIRQ(TIM5_IRQn);
}

void TIM5_IRQHandler(void)
{
  TIM5->SR = 0;
  TIM5->SR = 0;

  OneWireReset();
}
