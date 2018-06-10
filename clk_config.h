/*
 * clk_config.h
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */
#include "CMSIS/stm32l476xx.h"

#ifndef CLK_CONFIG_H_
#define CLK_CONFIG_H_

// default clk frequencies after reset
#define MSI_DEFAULT_FREQ        4000000UL

// default bus prescalers after reset
#define AHB_DEFAULT_PRESC       1UL
#define APB1_DEFAULT_PRESC      1UL
#define APB2_DEFAULT_PRESC      1UL

// default bus frequencies after reset
#define AHB_DEFAULT_FREQ          ((uint32_t)MSI_DEFAULT_FREQ / AHB_DEFAULT_PRESC)
#define APB1_DEFAULT_FREQ         ((uint32_t)MSI_DEFAULT_FREQ / APB1_DEFAULT_PRESC)
#define APB2_DEFAULT_FREQ         ((uint32_t)MSI_DEFAULT_FREQ / APB2_DEFAULT_PRESC)

#define RCC_CR_MSIRANGE_100kHz      RCC_CR_MSIRANGE_0
#define RCC_CR_MSIRANGE_200kHz      RCC_CR_MSIRANGE_1
#define RCC_CR_MSIRANGE_400kHz      RCC_CR_MSIRANGE_2
#define RCC_CR_MSIRANGE_800kHz      RCC_CR_MSIRANGE_3
#define RCC_CR_MSIRANGE_1MHz        RCC_CR_MSIRANGE_4
#define RCC_CR_MSIRANGE_2MHz        RCC_CR_MSIRANGE_5
#define RCC_CR_MSIRANGE_4MHz        RCC_CR_MSIRANGE_6
#define RCC_CR_MSIRANGE_8MHz        RCC_CR_MSIRANGE_7
#define RCC_CR_MSIRANGE_16MHz       RCC_CR_MSIRANGE_8
#define RCC_CR_MSIRANGE_24MHz       RCC_CR_MSIRANGE_9
#define RCC_CR_MSIRANGE_32MHz       RCC_CR_MSIRANGE_10
#define RCC_CR_MSIRANGE_48MHz       RCC_CR_MSIRANGE_11

typedef enum
{
  none,
  msi,
  hsi16,
  hsi48,
  hse,
  pll,
  lsi,
  lse,
}EnumClkType;

typedef enum
{
  ahb,
  apb1,
  apb2,
}EnumBusType;

typedef struct
{
  EnumClkType sys_clk_src;
  EnumClkType pll_src;          // zrodlo dla pll'a
  uint32_t sys_clk_freq;
  uint32_t ahb_freq;
  uint32_t apb1_freq;
  uint32_t apb2_freq;
}TClock;

typedef struct
{
  uint8_t msi_range;
  uint32_t freq;
}TMsiFreqConv;

void ClkInit(TClock * sys_clk_ptr);
uint32_t SetSystemClock(EnumClkType clk_src, uint32_t freq, TClock * sys_clk_ptr);
void SetBusFreq(EnumBusType bus, uint32_t freq);
void MsiSetup(EnumClkType clk_src, uint32_t freq, TClock * sys_clk_ptr);
void Hsi16Setup(EnumClkType clk_src, uint32_t freq, TClock * sys_clk_ptr);
void PllSetup(EnumClkType clk_src, uint32_t freq);


void SystemClockConfig(void);


#endif /* CLK_CONFIG_H_ */
