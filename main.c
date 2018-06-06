/*
 * main.c
 *
 *  Created on: 29 sie 2017
 *      Author: bwozniak
 */
#include <stdint.h>


#include "CMSIS/stm32l4xx.h"
#include "CMSIS/system_stm32l4xx.h"
#include "serial.h"
#include "clk_config.h"

/*
 * TIM2 period = 1 [Hz]
 */
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
/*
 * MSI 16MHz
 * APB1 prescaler : 4 -> APB1 CLK = 4MHz
 */
int main(void)
{
  SystemClockConfig();

  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

  GPIOE->MODER &= ~(3 << (2 * 8));
  GPIOE->MODER |= (1 << (2 * 8));


  TIM2_config();
  while (1);
}

