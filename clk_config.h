/*
 * clk_config.h
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */
#include "CMSIS/stm32l476xx.h"
#include "iButtonLogger.h"

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

#define MSI_FREQ_100kHz   100000UL
#define MSI_FREQ_200kHz   200000UL
#define MSI_FREQ_400kHz   400000UL
#define MSI_FREQ_800kHz   800000UL
#define MSI_FREQ_1MHz     1000000UL
#define MSI_FREQ_2MHz     2000000UL
#define MSI_FREQ_4MHz     4000000UL
#define MSI_FREQ_8MHz     8000000UL
#define MSI_FREQ_16MHz    16000000UL
#define MSI_FREQ_24MHz    24000000UL
#define MSI_FREQ_32MHz    32000000UL
#define MSI_FREQ_48MHz    48000000UL

#define MSI_FREQ_MIN      MSI_FREQ_100kHz
#define MSI_FREQ_MAX      MSI_FREQ_48MHz

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
}ESrcClkType;

typedef enum
{
  sysclk,
  hclk,
  pclk1,
  pclk2,
  adc_clk,
  sai1_clk,
  sai2_clk
}EPeriphClk;

typedef enum
{
  ahb,
  apb1,
  apb2,
}EBusType;

typedef struct
{
  ESrcClkType type;
  uint32_t freq;
}TSrcClk;

typedef struct
{
  EPeriphClk bus;       // szyna
  uint32_t bus_freq;    // czestotliwosc clk tej szyny
  TSrcClk src_clk;              // clk ktory ja napedza
  uint8_t presc;
}TPerpihClk;

typedef struct
{
  uint8_t msi_range_val;
  uint32_t freq;
}TMsiFreqConv;

void SystemClockConfig(void);


#endif /* CLK_CONFIG_H_ */
