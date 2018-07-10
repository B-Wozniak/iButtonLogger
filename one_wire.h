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
#define OW_I 79UL
#define OW_J 410UL
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
#define OW_PORT           GPIOA
#define OW_PIN            2
#define OW_PIN_DEF_CFG    GPIO_OUT_OD_2MHz    // for 1Hz 'search rom' polling

#define OW_LOW  (_set_low(OW_PORT, OW_PIN)) // pull 1Wire bus low
#define OW_HIGH (_set_high(OW_PORT, OW_PIN)) // release the bus
#define OW_INPUT_MODE (gpio_pin_cfg(OW_PORT, OW_PIN, GPIO_IN_FLOATING))
#define OW_OD_MODE (gpio_pin_cfg(OW_PORT, OW_PIN, GPIO_OUT_OD_2MHz))
#define OW_READ_BUS (_gpio_read(OW_PORT, OW_PIN))


typedef enum
{
  idle = 0,
  polling,
  write_rom_command,
  read,
}EOwState;

typedef struct
{
  TIM_TypeDef * timer;
  volatile uint32_t * presc_reg;
  uint32_t presc_val;
  uint32_t arr;
  IRQn_Type irqn;
  volatile uint32_t * clk_en_reg;
  uint32_t clk_en_val;
}TOneWireTimer;

typedef struct
{
  GPIO_TypeDef *port;
  uint8_t pin;
  uint32_t pin_cfg;
  TOneWireTimer timer;
}TOneWire;

typedef union
{
  struct
  {
    uint8_t crc;
    uint8_t serial_number[6];
    uint8_t family_code;
  }rom;
  uint64_t key;
}TIButtonKey;

void OwInit(void);

#endif /* ONE_WIRE_H_ */
