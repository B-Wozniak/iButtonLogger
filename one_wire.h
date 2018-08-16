/*
 * one_wire.h
 *
 *  Created on: 27 cze 2018
 *      Author: Bart
 */

#ifndef ONE_WIRE_H_
#define ONE_WIRE_H_

/* define 1-wire specific timings [us] */
#define OW_A 6UL
#define OW_B 64UL
#define OW_C 60UL
#define OW_D 10UL
#define OW_E 9UL
#define OW_F 55UL
#define OW_G 0UL
#define OW_H 480UL
#define OW_I 70UL
#define OW_J 410UL
#define OW_T_BIT (OW_A + OW_B)
#define OW_POLLING  1000000UL // 1s


/* state machine timer */
#define OW_TIM              TIM2
#define OW_TIM_EN_REG       &(RCC->APB1ENR1)
#define OW_TIM_EN_VAL       (RCC_APB1ENR1_TIM2EN)
#define OW_TIM_PRESC_1000   1000UL
#define OW_TIM_IRQN         TIM2_IRQn
#define OW_TIM_IRQn         TIM2_IRQn
#define OneWireIRQn    TIM2_IRQHandler

/* polling timer */
#define OW_POLL_TIM         TIM5
#define OW_POLL_TIM_EN_REG  &(RCC->APB1ENR1)
#define OW_POLL_TIM_EN_VAL  (RCC_APB1ENR1_TIM5EN)
#define OW_POLL_TIM_IRQn    TIM5_IRQn
#define OneWirePollIRQn        TIM5_IRQHandler

/* OneWire GPIO */
#define OW_PORT             GPIOD
#define OW_PIN              0
#define OW_PIN_DEF_CFG      GPIO_OUT_OD_100MHz    // for 1Hz 'search rom' polling
#define OW_LOW              _set_atomic_low(OW_PORT, OW_PIN)
#define OW_HIGH             _set_atomic_high(OW_PORT, OW_PIN)
#define OW_READ_BUS         _gpio_read(OW_PORT, OW_PIN)

#define SERIAL_NUMBER_SIZE 6  // bytes
#define KEY_SIZE  64 // ibutton key size in bits
#define KEY_SIZE_BYTE  8 // ibutton key size in bits

/* flag masks */
#define OW_BUTTON_READ_SUCCESS    0x01
#define OW_BUTTON_READ_CRC_ERROR  0x02
#define OW_POLL_EN 0x04

typedef enum
{
  idle = 0,
  reset,
  write,
  read,
}EOwState;

typedef union
{
  struct
  {
    uint8_t family_code;
    uint8_t serial_number[6];
    uint8_t crc;
  }rom_elem;
  uint8_t key_byte[8];
  uint64_t key;
}TIButton;

typedef enum
{
  read_rom,
  search_rom,
  match_rom,
  skip_rom,
  one_wire_commands // do not use, only size of enum
}EOwCmd;

extern volatile uint8_t ow_flags;
extern volatile EOwState one_wire_state;
extern TIButton ibutton;


void OneWireInit(void);
void OneWirePoll(void);
void OneWireReset(void);

#endif /* ONE_WIRE_H_ */
