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
  TIM2->PSC = (8000 * APB1_TIMER_MULT) - 1;
  TIM2->ARR = 1000 - 1;
  TIM2->CCR1 = 200;
  TIM2->CCR2 = 800;
  TIM2->CCR3 = 900;
  TIM2->DIER |= TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE; /* interrupt enable on 1st channel */
  TIM2->EGR = 1; /* event generation (to load all tim2 settings) */
  TIM2->SR = 0;
  NVIC_EnableIRQ(TIM2_IRQn);
  TIM2->CR1 |= TIM_CR1_CEN;
}

//void TIM2_IRQHandler(void)
//{
//  uint32_t sr;
//
//  sr = TIM2->SR & (TIM_SR_CC1IF | TIM_SR_CC2IF);
//  TIM2->SR = 0;
//
//  if (sr & TIM_SR_CC1IF)
//    _toggle_pin(GREEN_LED_PORT, GREEN_LED_PIN);
//
//  if (sr & TIM_SR_CC2IF)
//    _toggle_pin(RED_LED_PORT, RED_LED_PIN);
//}
