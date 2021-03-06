/*
 * clk_config.h
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */
#include "CMSIS/stm32l476xx.h"
#include "def.h"

#ifndef CLK_CONFIG_H_
#define CLK_CONFIG_H_

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

/* configured clocks */
#define SYSCLK          32000000UL  // MSI 32MHz
#define AHB_CLK         SYSCLK      // no presc
#define APB1_DIV        4UL
#define APB2_DIV        4UL
#define APB1_CLK        (SYSCLK / APB1_DIV)
#define APB2_CLK        (SYSCLK / APB2_DIV)

#define USE_APB1_DIV TRUE
#define USE_APB2_DIV TRUE

#if APB1_CLK < 1000000
#error apb1 clock should be at least 1MHz, 1Wire configuration requirement
#endif


#if USE_APB1_DIV == TRUE
#define APB1_TIMER_MULT 2
#else
#define APB1_TIMER_MULT 1
#endif

#if USE_APB2_DIV == TRUE
#define APB2_TIMER_MULT 2
#else
#define APB2_TIMER_MULT 1
#endif

void SystemClockConfig(void);


#endif /* CLK_CONFIG_H_ */
