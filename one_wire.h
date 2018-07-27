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


/* OneWire default settings */
#define OW_TIM              TIM2
#define OW_TIM_EN_REG       &(RCC->APB1ENR1)
#define OW_TIM_EN_VAL       (RCC_APB1ENR1_TIM2EN)
#define OW_TIM_PRESC_1000   1000UL

#define OW_IRQn TIM2_IRQn
#define OneWireInterrupt  TIM2_IRQHandler

#define OW_TIM_PRESC_VAL  31999UL
#define OW_TIM_ARR        1000UL
#define OW_TIM_IRQN       TIM2_IRQn
#define OW_PORT           GPIOD
#define OW_PIN            0
#define OW_PIN_DEF_CFG    GPIO_OUT_OD_100MHz    // for 1Hz 'search rom' polling

#define OW_LOW  _set_atomic_low(OW_PORT, OW_PIN)
#define OW_HIGH _set_atomic_high(OW_PORT, OW_PIN)

#define OW_READ_BUS (_gpio_read(OW_PORT, OW_PIN))

#define SERIAL_NUMBER_SIZE 6  // bytes
#define KEY_SIZE  64 // ibutton key size in bits

typedef enum
{
  idle = 0,
  polling,
  write,
  read,
  button_read
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

extern volatile EOwState one_wire_state;
extern TIButton ibutton;


void OneWireInit(void);
void OWPollingInit(void);
uint8_t CheckCrc(uint8_t *data);

#endif /* ONE_WIRE_H_ */
