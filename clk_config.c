/*
 * clk_config.c
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

/* every clk modification (msi freq, apb1/apb2 dividers)
 * requires changes in clk_config.h macros
 */
void SystemClockConfig(void)
{
  RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
  PWR->CR1 |= PWR_CR1_DBP;

  /* turn on LSE */
  RCC->BDCR |= RCC_BDCR_LSEON;
  while ((RCC->CR & RCC_BDCR_LSERDY) == 0);

  RCC->AHB1ENR |= RCC_AHB1ENR_FLASHEN;
  FLASH->ACR |= FLASH_ACR_LATENCY_4WS;

  /* turn on HSI clock and wait till HSI is stable*/
  RCC->CR |= RCC_CR_HSION;
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);

  /* select HSI as system clock */
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | RCC_CFGR_SW_HSI;
  while ((RCC->CFGR & RCC_CFGR_SWS_HSI) != RCC_CFGR_SWS_HSI);

  /* turn off MSI to set frequency */
  RCC->CR &= ~RCC_CR_MSION_Msk;

  /* set MSI to 4MHz */
  RCC->CR = (RCC->CR & ~RCC_CR_MSIRANGE_Msk) | RCC_CR_MSIRGSEL | RCC_CR_MSIPLLEN | RCC_CR_MSIRANGE_4MHz;
  RCC->CR |= RCC_CR_MSION;
  while ((RCC->CR & RCC_CR_MSIRDY) == 0);

  /* configure PLL */
  RCC->PLLCFGR = RCC_PLLCFGR_PLLREN | ((uint32_t)40 << RCC_PLLCFGR_PLLN_Pos) | RCC_PLLCFGR_PLLSRC_MSI;
  RCC->CR |= RCC_CR_PLLON;

  while ((RCC->CR & RCC_CR_PLLRDY) == 0);

  /* select PLL as system clock */
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | RCC_CFGR_SW_PLL | RCC_CFGR_PPRE1_DIV8;
  while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

  /* turn off HSI */
  RCC->CR &= ~(RCC_CR_HSION);
}
