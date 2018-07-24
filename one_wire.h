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

//#define OW_LOW  (_set_low(OW_PORT, OW_PIN)) // pull 1Wire bus low
//#define OW_HIGH (_set_high(OW_PORT, OW_PIN)) // release the bus
//TODO: do sprawdzenia zapisy atomowe !
#define OW_LOW  _set_atomic_low(OW_PORT, OW_PIN)
#define OW_HIGH _set_atomic_high(OW_PORT, OW_PIN)

// przelaczanie w input mode nie jest potrzebne !
// przy konfiguracji pinnu jako OUTPUT (moze byc OD), wejsciowy
// przerzutnik shmidta i tak jest aktywny i sampluje co period AHB (RM 8.3.10)
//#define OW_INPUT_MODE (OW_PORT->MODER &= ~(1 << (OW_PIN * 2)))
//#define OW_OD_MODE    (OW_PORT->MODER |= (1 << (OW_PIN * 2)))
//a moze jednak tak ? zawsze bezpieczniej czyscic najpierw ustawiane bity, mniejsze ryzyko pomylki
//#define OW_INPUT_MODE()   do { OW_PORT->MODER &= ~(GPIO_MODER_mask << (OW_PIN * 2)); OW_PORT->MODER |= (GPIO_MODER_IN_value << (OW_PIN * 2)));  } while (0)
//#define OW_OD_MODE()      do { OW_PORT->MODER &= ~(GPIO_MODER_mask << (OW_PIN * 2)); OW_PORT->MODER |= (GPIO_MODER_OUT_value << (OW_PIN * 2))); } while (0)

#define OW_READ_BUS (_gpio_read(OW_PORT, OW_PIN))

#define SERIAL_NUMBER_SIZE 6
//TODO: sizeof(TIButtonKey) ? sprawdzic padding i czy na pewno bedzie 8 bajtow
#define KEY_SIZE  8 // ibutton key size in bytes

typedef enum
{
  idle = 0,
  polling,
  write,
  read,
}EOwState;

typedef union
{
  struct
  {
    uint8_t crc;
    uint8_t serial_number[SERIAL_NUMBER_SIZE];
    uint8_t family_code;
  }rom;
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

void OWInit(void);

#endif /* ONE_WIRE_H_ */
