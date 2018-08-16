/*
 * one_wire.c
 *
 *  Created on: 27 cze 2018
 *      Author: Bart
 *
 *  TODO:
 *  - polling na one pulse mode ? tak chyba powinno byc
 */

#include "iButtonLogger.h"

/* macros to PSC, ARR val -> to not forget ( -1 ) and double clock freq if apb_div != 1 */
#define _arr_val(val) ((val) - 1)
#define _apb1_tim_psc_val(val) (val * APB1_TIMER_MULT - 1)
#define _ow_sample ()

inline static void  OneWirePollStop(void);
static void OWWriteInit(EOwCmd cmd);
static void OWReadInit(void);
static void OwStart(void);
static uint8_t _crc_ibutton_update(uint8_t crc, uint8_t data);
static uint8_t CheckCrc(uint8_t *data);

static const uint8_t one_wire_cmd_values[one_wire_commands] = {0x33, 0xF0, 0x55, 0xCC};
static const uint8_t * ow_cmd = NULL;

static volatile EOwState one_wire_next_state = idle;
volatile EOwState one_wire_state = idle;
TIButton ibutton;

volatile uint8_t ow_flags;

static uint8_t CheckCrc(uint8_t *data)
{
  uint8_t i, crc;

  crc = 0x00;

  for (i = 0; i < 8; i++)
    crc = _crc_ibutton_update(crc, data[i]);

  return crc == 0 ? SUCCESS : ERROR;
}

static uint8_t _crc_ibutton_update(uint8_t crc, uint8_t data)
{
  uint8_t i;

  crc = crc ^ data;

  for (i = 0; i < 8; i++)
  {
    if (crc & 0x01)
      crc = (crc >> 1) ^ 0x8C;
    else
      crc >>= 1;
  }
  return crc;
}

void OneWireInit(void)
{
  /* configure OneWire port and pin*/
  gpio_pin_cfg(OW_PORT, OW_PIN, OW_PIN_DEF_CFG);

  /* configure state machine timer */
  *OW_TIM_EN_REG |= OW_TIM_EN_VAL;
  OW_TIM->PSC = _apb1_tim_psc_val(80);  /* 1MHz */
  OW_TIM->CR1 |= TIM_CR1_OPM;
  OW_TIM->DIER = TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE;
  NVIC_EnableIRQ(OW_TIM_IRQn);

  /* configure polling timer */
  *OW_POLL_TIM_EN_REG |= OW_POLL_TIM_EN_VAL;
  OW_POLL_TIM->PSC = (80 * APB1_TIMER_MULT) - 1;
  OW_POLL_TIM->ARR = 1000000 - 1;     /* 1Hz */
  OW_POLL_TIM->DIER = TIM_DIER_UIE;
  NVIC_EnableIRQ(OW_POLL_TIM_IRQn);
}

inline void OneWirePoll(void)
{
  ow_flags |= OW_POLL_EN;
  _set_high(RED_LED_PORT, RED_LED_PIN);
  OW_POLL_TIM->CR1 |= TIM_CR1_CEN;
}

inline static void OneWirePollStop(void)
{
  OW_POLL_TIM->CR1 &= ~TIM_CR1_CEN_Msk;
  OW_POLL_TIM->CNT = 0;
  OW_POLL_TIM->SR = 0;
}

/*
 * @brief Init one wire state machine: polling
 * Each second master pulls bus low and awaits for presence pulls from the slave. Routine:
 * - start: count to 1 second, then drive bus low <-- that's the added part, rest is standard 1wire signaling
 * - count to OW_H = 480us, then release bus, then switch pin to input mode
 * - count to OW_C = 60us, then test bus for the presence pulse
 * - if presence pulse detected, switch to write state
 */
void OneWireReset(void)
{
  if (one_wire_state != reset)
  {
    one_wire_state = reset;
    OW_TIM->CCR1 = OW_H;
    OW_TIM->CCR2 = OW_H + OW_I;
    OW_TIM->ARR = _arr_val(OW_H + OW_I + OW_J);
  }
  OwStart();
}

/*
 * @brief Init one wire state machine: write
 */

static void OWWriteInit(EOwCmd cmd)
{
  one_wire_state = write;
  ow_cmd = &one_wire_cmd_values[cmd];

  OW_TIM->CCR1 = OW_A;  // write 1 time
  OW_TIM->CCR2 = OW_C;  // write 0 time
  OW_TIM->ARR = _arr_val(OW_T_BIT);

  OwStart();
}

/*
 * @brief Init one wire state machine: read
 */

static void OWReadInit(void)
{
  one_wire_state = read;

  OW_TIM->CCR1 = OW_A;
  OW_TIM->CCR2 = OW_A + OW_E;
  OW_TIM->ARR = _arr_val(OW_T_BIT);

  OwStart();
}

/*
 * @brief Trigger for one wire timer
 */

static void OwStart(void)
{
  OW_TIM->SR = 0;
  OW_LOW;
  OW_TIM->CR1 |= TIM_CR1_CEN;
}

// reset state
#define _reset_time                 TIM_SR_CC1IF
#define _presence_sample            TIM_SR_CC2IF
#define _reset_end                  TIM_SR_UIF
// write state
#define _write_one_time             TIM_SR_CC1IF
#define _write_zero_time            TIM_SR_CC2IF
// read state
#define _read_bus_release           TIM_SR_CC1IF
#define _read_sample                TIM_SR_CC2IF
// read, write common
#define _bit_end                    TIM_SR_UIF

void OneWireIRQn(void)
{
  uint32_t sr;
  static uint8_t data_byte;
  static uint8_t bit_pos;
  static uint8_t bit_cnt;

  sr = OW_TIM->SR & (TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_UIF);
  OW_TIM->SR = 0;

  /* read 1wire bus */
  uint32_t bus_state = OW_READ_BUS;

  /* depending on 1Wire state, take the action */
  switch(one_wire_state)
  {
    case reset:
    {
      switch(sr)
      {
        case _reset_time:
        {
          /* reset time elapsed, release the bus,
           * master waits for presence pulse from slave
           */
          OW_HIGH;
        }
        break;

        case _presence_sample:
        {
          /* it is time to sample bus */
          if (!bus_state)
          {
            OneWirePollStop();
            /* presence pulse detected */
            _set_low(RED_LED_PORT, RED_LED_PIN);
            one_wire_next_state = write;
          }
        }
        break;

        case _reset_end:
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
            bit_cnt = 0;
            if (CheckCrc(ibutton.key_byte) == SUCCESS)
            {
              _set_high(GREEN_LED_PORT, GREEN_LED_PIN);
              ow_flags |= OW_BUTTON_READ_SUCCESS;
            }
            else
              ow_flags |= OW_BUTTON_READ_CRC_ERROR;
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

void OneWirePollIRQn(void)
{
  TIM5->SR = 0;
  TIM5->SR = 0;

  OneWireReset();
}
