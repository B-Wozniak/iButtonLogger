/*
 * one_wire.c
 *
 *  Created on: 27 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

static volatile EOwState OneWireState = idle;

/* macros to PSC, ARR val -> to not forget ( -1 ) */
#define _arr_val(val) ((val) - 1)
#define _psc_val(val) (val * APB1_TIMER_MULT - 1)

static void OWPollingInit(void);
static void OWWriteInit(void);
static void OWReadInit(void);

void OwInit(void)
{
  /* configure OneWire port and pin*/
  gpio_pin_cfg(OW_PORT, OW_PIN, OW_PIN_DEF_CFG);

  /* enable OneWire clock in RCC register*/
  *OW_TIM_EN_REG |= OW_TIM_EN_VAL;

  /* 1Wire timer == 1MHz */
  OW_TIM->PSC = _psc_val(8);
  OW_TIM->DIER |= TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE;

  NVIC_EnableIRQ(OW_IRQn);
  OWPollingInit();
}

void OWPollingInit(void)
{
  OneWireState = polling;

  OW_TIM->CCR1 = OW_POLLING;
  OW_TIM->CCR2 = OW_POLLING + OW_H;
  OW_TIM->ARR = _arr_val(OW_POLLING + OW_H + OW_C);
  OW_TIM->CR1 |= TIM_CR1_CEN;
}

void OWWriteInit(void)
{
  /* set timer to one pulse mode */
  OW_TIM->CR1 |= TIM_CR1_OPM;

  OW_TIM->CCR1 = OW_POLLING;
  OW_TIM->CCR2 = OW_POLLING + OW_H;
  OW_TIM->ARR = _arr_val(OW_POLLING + OW_H + OW_C);
  OW_TIM->CR1 |= TIM_CR1_CEN;
}

void OWReadInit(void)
{

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
  sr = OW_TIM->SR & (TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_UIF);
  OW_TIM->SR = 0;

  /* depending on 1Wire state, take the action */
  switch(OneWireState)
  {
    case polling:
    {
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
          GPIOA->MODER &= ~(3 << (2*2));
        }
        break;

        case TIM_SR_UIF:
        {
          /* here, we should be in the middle of slave presence pulse */

          if (!OW_READ_BUS)
          {
            /* presence pulse detected */
            _set_low(RED_LED_PORT, RED_LED_PIN);
            OneWireState = write;
            sr = 0;
            OWWriteInit();
          }
          else
            /* no device on bus, turn on red led */
            _set_high(RED_LED_PORT, RED_LED_PIN);

          /* set 1Wire pin back to OD mode */
          OW_OD_MODE;
          OW_HIGH;
        }
        break;
      }
    }
    break;

    case read:
    {
      switch(sr)
      {
        case TIM_SR_CC1IF:
          _set_low(RED_LED_PORT, RED_LED_PIN);
          _toggle_pin(GREEN_LED_PORT, GREEN_LED_PIN);
        break;
      }
    }
    break;

    case write:
    {
      switch(sr)
      {
        case TIM_SR_CC1IF:
          _set_low(RED_LED_PORT, RED_LED_PIN);
          _toggle_pin(GREEN_LED_PORT, GREEN_LED_PIN);
        break;
      }
    }
    break;

    default:
      break;
  }
}
