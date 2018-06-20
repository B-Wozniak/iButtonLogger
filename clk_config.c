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

//                               BUS       BUS_FREQ     BUS_SRC_CLK   BUS_SRC_CLK_FREQ    BUS_PRESC
const TPerpihClk clk_defaults = {sysclk,   4000000UL,  {msi,          MSI_FREQ_16MHz},    4};

static const TMsiFreqConv msiconvtable[] =
{
    {0,   MSI_FREQ_100kHz},
    {1,   MSI_FREQ_200kHz},
    {2,   MSI_FREQ_400kHz},
    {3,   MSI_FREQ_800kHz},
    {4,   MSI_FREQ_1MHz},
    {5,   MSI_FREQ_2MHz},
    {6,   MSI_FREQ_4MHz},
    {7,   MSI_FREQ_8MHz},
    {8,   MSI_FREQ_16MHz},
    {9,   MSI_FREQ_24MHz},
    {10,  MSI_FREQ_32MHz},
    {11,  MSI_FREQ_48MHz}
};

#define _IsPllSrcOk(clk) (clk == msi    ||      \
                          clk == hsi16  ||      \
                          clk == hse ? 1 : 0)

uint8_t ConvMsiFreqToRegVal(uint32_t msi_freq)
{
  uint8_t i;

  if (msi_freq < MSI_FREQ_MIN ||
      msi_freq > MSI_FREQ_MAX)
    return UINT8_MAX;

  for (i = 0; i < _num_of_elem(msiconvtable); i++)
    if (msiconvtable[i].freq == msi_freq)
      return msiconvtable[i].msi_range_val;

  return UINT8_MAX;
}

/*
 * @brief fill the System Clock structure after MCU reset
 */
//void ClkInit(TClock * sys_clk_ptr)
//{
//  /* MSI 4MHz - default sysclk frequency after reset */
//  sys_clk_ptr->sys_clk_src = msi;
//  sys_clk_ptr->sys_clk_freq = MSI_DEFAULT_FREQ;
//
//  /* define default bus frequencies */
//  sys_clk_ptr->ahb_freq = AHB_DEFAULT_FREQ;
//  sys_clk_ptr->apb1_freq = APB1_DEFAULT_FREQ;
//  sys_clk_ptr->apb2_freq = APB2_DEFAULT_FREQ;
//
//  /* by default, pll is off */
//  sys_clk_ptr->pll_src = none;
//}

void MsiSetup(TPerpihClk * clk_conf)
{
  /* check if settings already set = nothing to do */
//  if (sys_clk_ptr->sys_clk_src == clk_src &&
//      sys_clk_ptr->sys_clk_freq == freq)
//    return;

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

  /* set MSI to desired frequency */
  RCC->CR = ConvMsiFreqToRegVal(clk_conf->src_clk.freq);
  RCC->CR |= RCC_CR_MSION;
  while ((RCC->CR & RCC_CR_MSIRDY) == 0);

  /* select MSI as system clock */
  RCC->CFGR &= ~(RCC_CFGR_SW_Msk);
  RCC->CFGR |= RCC_CFGR_SW_MSI;
  while (RCC->CFGR & RCC_CFGR_SWS_MSI);

  /* turn off HSI */
  RCC->CR &= ~(RCC_CR_HSION);
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
