/*
 * one_wire.c
 *
 *  Created on: 27 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

static EOwState OneWireState = polling;

void OwInit(void)
{
  /* configure OneWire port and pin*/
  gpio_pin_cfg(OW_PORT, OW_PIN, OW_PIN_DEF_CFG);

  /* enable OneWire clock in RCC register*/
  *OW_TIM_EN_REG |= OW_TIM_EN_VAL;

#if APB1_CLK < 1000000
#error apb1 clock should be at least 1MHz
#endif

/* macros to PSC, ARR val -> to not forget ( -1 ) */
#define _psc_arr_val(val) ((val) - 1)

  /* configure timer for 1sec polling */
  OW_TIM->PSC = (8000 * APB1_TIMER_MULT) - 1; //  1MHz
  OW_TIM->CCR1 = OW_POLLING;
  OW_TIM->CCR2 = OW_POLLING + OW_H;
  OW_TIM->ARR = _psc_arr_val(OW_POLLING + OW_H + OW_C);
  OW_TIM->DIER |= TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE;
  OW_TIM->EGR = 1;
  OW_TIM->SR = 0;
  NVIC_EnableIRQ(OW_IRQn);
  OW_TIM->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void)
{
  /*
   * polling state:
   *  each second master pulls bus low (ow_reset) and awaits for presence pulls from the slave, so:
   *    - start: count to 1 second (CCR1), then drive bus low
   *    - count to OW_H = 480us (CCR2), then release bus, then switch pin to input mode
   *    - count to OW_C = 60us (ARR), then test bus for the presence pulse
   *
   */
  uint32_t sr;

  /* copy status register */
  sr = OW_TIM->SR;
  OW_TIM->SR = 0;

  /* depending on 1Wire state, take the action */
  switch(OneWireState)
  {
    case polling:
      switch(sr)
      {
        case TIM_SR_CC1IF:
        {
          /* polling time elapsed, start reset -> pull bus low */
          OW_LOW;
        }
        break;

        case TIM_SR_CC2IF:
        {
          /* reset time elapsed, release the bus, change pin mode to input,
           * master waits for presence pulse from slave
           */
          OW_HIGH;
          OW_INPUT_MODE;
        }
        break;

        case TIM_SR_UIF:
        {
          /* here, we should be in the middle of slaves presence pulse, sample it */
          if (!OW_READ_BUS)
            _set_high(GREEN_LED_PORT, GREEN_LED_PIN);

          /* set 1Wire pin back to OD mode */
          OW_OD_MODE;
        }
        break;
      }
    break;

    default:
      break;
  }
}
