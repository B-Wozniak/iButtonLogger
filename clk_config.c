/*
 * clk_config.c
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

void SystemClockConfig(void)
{
  /* turn on HSI clock and wait till HSI is stable*/
  RCC->CR |= RCC_CR_HSION;
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);

  /* select HSI as system clock */
  RCC->CFGR |= RCC_CFGR_SW_HSI;
  while ((RCC->CFGR & RCC_CFGR_SWS_HSI) == 0);

  /* turn off MSI to set frequency */
  RCC->CR &= ~(RCC_CR_MSION);
  RCC->CR |= RCC_CR_MSIRGSEL;
  __DSB();

  /* set MSI to 16MHz */
  RCC->CR = RCC_CR_MSIRANGE_32MHz;
  RCC->CR |= RCC_CR_MSION;
  while ((RCC->CR & RCC_CR_MSIRDY) == 0);

  /* select MSI as system clock */
  RCC->CFGR &= ~(RCC_CFGR_SW_Msk);
  RCC->CFGR |= RCC_CFGR_SW_MSI;
  while (RCC->CFGR & RCC_CFGR_SWS_MSI);

  /* turn off HSI */
  RCC->CR &= ~(RCC_CR_HSION);

  /* set buses frequency */
//  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV4;
}
