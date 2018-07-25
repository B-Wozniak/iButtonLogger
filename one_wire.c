/*
 * one_wire.c
 *
 *  Created on: 27 cze 2018
 *      Author: Bart
 *
 *  TODO:
 *  - OwStart() - zamiast tego po prostu jedna instrukcja wlaczac timer ?
 *  - bariery - sprawdzic poprawnosc kasowania flag statusu przerwan (OW_TIM->SR),
 *              czy przypadkiem nie trzeba uzyc barier
 */

#include "iButtonLogger.h"

/* macros to PSC, ARR val -> to not forget ( -1 ) */
#define _arr_val(val) ((val) - 1)
#define _psc_val(val) (val * APB1_TIMER_MULT - 1)
#define _ow_sample ()

static void OWPollingInit(void);
static void OWWriteInit(EOwCmd cmd);
static void OWReadInit(void);
static void OwStart(void);

static volatile TIButton ibutton;

static volatile EOwState one_wire_next_state = idle;
static const uint8_t one_wire_cmd_values[one_wire_commands] = {0x33, 0xF0, 0x55, 0xCC};

static const uint8_t * ow_cmd = NULL;

volatile EOwState one_wire_state = idle;

void OWInit(void)
{
  /* configure OneWire port and pin*/
  gpio_pin_cfg(OW_PORT, OW_PIN, OW_PIN_DEF_CFG);

  /* enable OneWire clock in RCC register*/
  *OW_TIM_EN_REG |= OW_TIM_EN_VAL;

  OW_TIM->CR1 |= TIM_CR1_URS;

  /* 1Wire timer == 1MHz */
  OW_TIM->PSC = 80 - 1;

  /* turn on irqn's */
  NVIC_EnableIRQ(OW_IRQn);

  /* init 1-sec polling (search for slave)*/
  OWPollingInit();
}


/*
 * @brief Init one wire state machine: polling
 * Each second master pulls bus low and awaits for presence pulls from the slave. Routine:
 * - start: count to 1 second, then drive bus low <-- that's the added part, rest is standard 1wire signaling
 * - count to OW_H = 480us, then release bus, then switch pin to input mode
 * - count to OW_C = 60us, then test bus for the presence pulse
 * - if presence pulse detected, switch to write state
 */
void OWPollingInit(void)
{
  one_wire_state = polling;

  /* enable irqns on channels */
  OW_TIM->DIER = TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE;

  OW_TIM->CCR1 = OW_POLLING;
  OW_TIM->CCR2 = OW_POLLING + OW_H;
  OW_TIM->CCR3 = OW_POLLING + OW_H + OW_I;
  OW_TIM->ARR = _arr_val(OW_POLLING + OW_H + OW_I + OW_J);

  OwStart();
}

/*
 * @brief Init one wire state machine: write
 */

void OWWriteInit(EOwCmd cmd)
{
  one_wire_state = write;
  ow_cmd = &one_wire_cmd_values[cmd];

  OW_TIM->DIER = TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE;
  OW_TIM->CCR1 = OW_A;  // write 1 time
  OW_TIM->CCR2 = OW_C;  // write 0 time
  OW_TIM->ARR = _arr_val(OW_T_BIT);

  OwStart();
}

/*
 * @brief Init one wire state machine: read
 */

void OWReadInit(void)
{
  one_wire_state = read;

  OW_TIM->DIER = TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE;
  OW_TIM->CCR1 = OW_A;
  OW_TIM->CCR2 = OW_A + OW_E;
  OW_TIM->ARR = _arr_val(OW_T_BIT);

  OwStart();
}

/*
 * @brief Trigger for one wire timer
 */

void OwStart(void)
{
  OW_TIM->SR = 0;
  if (one_wire_state != polling)
    OW_LOW;
  OW_TIM->CR1 |= TIM_CR1_CEN;
}

// polling state
#define _polling_time               TIM_SR_CC1IF
#define _polling_reset_time         TIM_SR_CC2IF
#define _polling_presence_sample    TIM_SR_CC3IF
#define _poll_and_reset_end         TIM_SR_UIF

// write state
#define _write_one_time             TIM_SR_CC1IF
#define _write_zero_time            TIM_SR_CC2IF

// read state
#define _read_bus_release           TIM_SR_CC1IF
#define _read_sample                TIM_SR_CC2IF

// read, write common
#define _bit_end                    TIM_SR_UIF

void OneWireInterrupt(void)
{
  uint32_t sr;
  static uint8_t data_byte;
  static uint8_t bit_pos;
  static uint8_t bit_cnt;

  /* read 1wire bus */
  uint32_t bus_state = OW_READ_BUS;

  /* depending on 1Wire state, take the action */
  switch(one_wire_state)
  {
    case polling:
    {
      sr = OW_TIM->SR & (TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_UIF);
      OW_TIM->SR = 0;

      switch(sr)
      {
        case _polling_time:
        {
          /* polling time elapsed, start reset sequence -> pull bus low */
          OW_LOW;
        }
        break;

        case _polling_reset_time:
        {
          /* reset time elapsed, release the bus, change pin mode to input,
           * master waits for presence pulse from slave
           */
          OW_HIGH;
        }
        break;

        case _polling_presence_sample:
        {
          /* it is time to sample bus */
          if (!bus_state)
          {
            /* presence pulse detected */
            _set_low(RED_LED_PORT, RED_LED_PIN);
            one_wire_next_state = write;
            OW_TIM->CR1 |= TIM_CR1_OPM;
          }
          else
          {
            /* no device on bus, turn on red led */
            _set_high(RED_LED_PORT, RED_LED_PIN);
          }
        }
        break;

        case _poll_and_reset_end:
        {
          /* end of reset sequence, do nothing if no device on bus*/
          if (one_wire_next_state == write)
          {
            /* start write sequence */
            OWWriteInit(read_rom);
          }
        }
        break;
      }
    }
    break;

    case read:
    {
      sr = OW_TIM->SR & (TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_UIF);
      OW_TIM->SR = 0;

      switch(sr)
      {
        case _read_bus_release:
        {
          OW_HIGH;  // release bus
        }
        break;

        case _read_sample:
        {
          if (bus_state)
            data_byte |= (1 << bit_pos);
        }
        break;

        case _bit_end:
        {
          bit_pos++;

          /* one wire commands always 1-byte long */
          if (bit_pos == 8)
          {
            ibutton.key |= (uint64_t)data_byte << bit_cnt;
            bit_pos = 0;
            data_byte = 0;
            bit_cnt+=8;
          }

          if (bit_cnt == KEY_SIZE)
          {
            /* key received, end of transmission */
            one_wire_state = idle;
            bit_cnt = 0;
          }
          else
            OwStart(); // keep rolling
        }
        break;
      }
    }
    break;

    case write:
    {
      sr = OW_TIM->SR & (TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_UIF);
      OW_TIM->SR = 0;

      switch(sr)
      {
        case _write_one_time:
          if (((*ow_cmd >> bit_pos) & 1) == 0)
            break;
          /* FALLTHROUGH */
        case _write_zero_time:
          OW_HIGH;
        break;

        case _bit_end:
        {
          bit_pos++;

          /* one wire commands always 1-byte long */
          if (bit_pos < 8)
            OwStart();
          else
          {
            bit_pos = 0;
            OWReadInit();
          }
        }
        break;
      }
    }
    break;

    case idle:
    default:
    break;
  }
}

