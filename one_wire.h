/*
 * one_wire.h
 *
 *  Created on: 27 cze 2018
 *      Author: 710_080370
 */

#ifndef ONE_WIRE_H_
#define ONE_WIRE_H_

/* OneWire Timer settings */
#define TIM2_CLK_EN_REG &(RCC->APB1ENR1)
#define TIM2_CLK_EN_VAL (RCC_APB1ENR1_TIM2EN)

typedef enum
{
  idle = 0,
  write,
  read,
  search
}EOwState;

typedef struct
{
  TIM_TypeDef * timer;
  uint32_t presc;
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

#endif /* ONE_WIRE_H_ */
