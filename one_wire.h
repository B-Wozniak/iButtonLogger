/*
 * one_wire.h
 *
 *  Created on: 27 cze 2018
 *      Author: Bart
 */

#ifndef ONE_WIRE_H_
#define ONE_WIRE_H_

/* define 1-wire specific timings [us] */
#define OW_A 6
#define OW_B 64
#define OW_C 60
#define OW_D 10
#define OW_E 9
#define OW_F 55
#define OW_G 0
#define OW_H 480
#define OW_I 79
#define OW_J 410
#define OW_POLLING  1000000 // 1s


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
#define OW_PIN_DEF_CFG    GPIO_OUT_OD_100MHz    // for 1Hz 'search rom' polling

typedef enum
{
  idle = 0,
  polling,
  write,
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
