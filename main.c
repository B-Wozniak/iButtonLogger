/*
 * main.c
 *
 *  Created on: 29 sie 2017
 *      Author: bwozniak
 */
#include <stdint.h>

#include "stm32l4xx.h"

/*
 * TIM2 period = 2 [Hz]
 */
void TIM2_config(void)
{
  TIM2->PSC = 1000;
  TIM2->ARR = 2000;
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
/*
 * MSI 4MHz (default)
 * APB1 prescaler : 4 -> APB1 CLK = 1MHz
 */
int main(void)
{
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
  RCC->CFGR |= RCC_CFGR_PPRE1_0 | RCC_CFGR_PPRE1_2;
  GPIOE->MODER &= ~(3 << (2 * 8));
  GPIOE->MODER |= (1 << (2 * 8));

  TIM2_config();
  while (1);
}

