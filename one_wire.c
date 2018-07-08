/*
 * one_wire.c
 *
 *  Created on: 27 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

#define _ow_release gpio_pin_cfg(OW_PORT, OW_PIN, GPIO_OUT_OD_100MHz)
//#define _ow_set_low
//#define _ow_input

//void ConfigureOneWire(void)
//{
//  /* configure OneWire port and pin*/
//  gpio_pin_cfg(OW_PORT, OW_PIN, OW_PIN_DEF_CFG);
//
//  /* enable OneWire clock in RCC register*/
//  *OW_TIM_EN_REG |= OW_TIM_EN_VAL;
//
//
//  /* configure timer for 1sec polling */
//  OW_TIM->PSC = 3; //  1MHz
//  OW_TIM->CCR1 = OW_POLLING;
//  OW_TIM->CCR2 = OW_POLLING + OW_H;
//  OW_TIM->ARR = OW_POLLING + OW_H + OW_C;
//  OW_TIM->DIER |= TIM_DIER_UIE;
//  OW_TIM->EGR = 1;
//  OW_TIM->SR = 0;
//  NVIC_EnableIRQ(OW_IRQn);
//  OW_TIM->CR1 |= TIM_CR1_CEN;
//}
//
//void OneWireInterrupt(void)
//{
//  /*
//   * polling state:
//   *  each second master pulls bus low (ow_reset) and awaits for presence pulls from the slave, so:
//   *    - start: count to 1 second (CCR1), then drive bus low
//   *    - count to OW_H = 480us (CCR2), then release bus, then switch pin to input mode
//   *    - count to OW_C = 60us (ARR), then test bus for the presence pulse
//   *
//   */
//  uint16_t sr;
//}


