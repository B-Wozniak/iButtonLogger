/*
 * clk_config.c
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

// SystemConfig(clk_src, freq, ahb_freq, apb1_freq, apb2_freq)
// if (clk_src == pll)
//    odpal osobna funkcje, ktora konfiguruje pll'a
// tutaj dopiero konfiguracja poszczegolnych szyn
// potrzebuje cos zwracac z ten fukncji ? sys_clk ?
// jesli dla ktorejs z szyn jako parametr damy 0 - > nie ruszac, zostawic domyslne ?


/*
 * @brief function to choose clock and frequency
 *
 * @param clk_source
 */

static const TMsiFreqConv convtable[] =
{
    {0,   100000},    // 100kHz
    {1,   200000},    // 200kHz
    {2,   400000},    // 400kHz
    {3,   800000},    // 800kHz
    {4,   1000000},   // 1MHz
    {5,   2000000},   // 2MHz
    {6,   4000000},   // 4MHz
    {7,   8000000},   // 8MHz
    {8,   16000000},  // 16MHz
    {9,   24000000},  // 24MHz
    {10,  32000000},  // 32MHz
    {11,  48000000}   // 48MHz
};

#define _IsPllSrcOk(clk) (clk == msi    ||      \
                          clk == hsi16  ||      \
                          clk == hse ? 1 : 0)

/*
 * @brief fill the System Clock structure after MCU reset
 */
void ClkInit(TClock * sys_clk_ptr)
{
  /* MSI 4MHz - default sysclk frequency after reset */
  sys_clk_ptr->sys_clk_src = msi;
  sys_clk_ptr->sys_clk_freq = MSI_DEFAULT_FREQ;

  /* define default bus frequencies */
  sys_clk_ptr->ahb_freq = AHB_DEFAULT_FREQ;
  sys_clk_ptr->apb1_freq = APB1_DEFAULT_FREQ;
  sys_clk_ptr->apb2_freq = APB2_DEFAULT_FREQ;

  /* by default, pll is off */
  sys_clk_ptr->pll_src = none;
}

uint32_t SetSystemClock(EnumClkType clk_src, uint32_t freq, TClock * sys_clk_ptr)
{
  switch (clk_src)
  {
    case msi:
      MsiSetup(clk_src, freq, sys_clk_ptr);
    break;

    case hsi16:
      hsi16_setup();
    break;
  }
  return 0;
}

void MsiSetup(EnumClkType clk_src, uint32_t freq, TClock * sys_clk_ptr)
{
  /* check if settings already set = nothing to do */
  if (sys_clk_ptr->sys_clk_src == clk_src &&
      sys_clk_ptr->sys_clk_freq == freq)
    return;

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
  RCC->CR = RCC_CR_MSIRANGE_16MHz;
  RCC->CR |= RCC_CR_MSION;
  while ((RCC->CR & RCC_CR_MSIRDY) == 0);

  /* select MSI as system clock */
  RCC->CFGR &= ~(RCC_CFGR_SW_Msk);
  RCC->CFGR |= RCC_CFGR_SW_MSI;
  while (RCC->CFGR & RCC_CFGR_SWS_MSI);

  /* turn off HSI */
  RCC->CR &= ~(RCC_CR_HSION);
}

void Hsi16Setup(EnumClkType clk_src, uint32_t freq, TClock * sys_clk_ptr)
{

}

void PllSetup(EnumClkType clk_src, uint32_t freq)
{
  // verify freq
}

void SetBusFreq(EnumBusType bus, uint32_t freq)
{

}


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
  RCC->CR = RCC_CR_MSIRANGE_16MHz;
  RCC->CR |= RCC_CR_MSION;
  while ((RCC->CR & RCC_CR_MSIRDY) == 0);

  /* select MSI as system clock */
  RCC->CFGR &= ~(RCC_CFGR_SW_Msk);
  RCC->CFGR |= RCC_CFGR_SW_MSI;
  while (RCC->CFGR & RCC_CFGR_SWS_MSI);

  /* turn off HSI */
  RCC->CR &= ~(RCC_CR_HSION);
}
