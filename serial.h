/*
 * serial.h
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */

#ifndef SERIAL_H_
#define SERIAL_H_

/* USART2 SETTINGS */
#define USART2_TX_PORT      GPIOD
#define USART2_RX_PORT      GPIOD
#define USART2_TX_PIN       5
#define USART2_RX_PIN       6
#define USART2_CLK_EN_REG   &(RCC->APB1ENR1)
#define USART2_CLK_EN_VAL   (RCC_APB1ENR1_USART2EN)
#define USART2_BR           9600UL
#define USART2_CLK          APB1_CLK
#define USART2_TX_PIN_CFG   GPIO_AF7_PP_2MHz
#define USART2_RX_PIN_CFG   GPIO_AF7_PP_2MHz

/* USART3 SETTINGS */
#define USART3_TX_PORT      GPIOD
#define USART3_RX_PORT      GPIOD
#define USART3_TX_PIN       8
#define USART3_RX_PIN       9
#define USART3_CLK_EN_REG   &(RCC->APB1ENR1)
#define USART3_CLK_EN_VAL   (RCC_APB1ENR1_USART3EN)
#define USART3_BR           115200UL
#define USART3_CLK          APB1_CLK
#define USART3_TX_PIN_CFG   GPIO_AF7_OD_2MHz
#define USART3_RX_PIN_CFG   GPIO_AF7_OD_2MHz

#define BUFF_SIZE 128UL
#define BUFF_MASK (BUFF_SIZE - 1)

typedef struct
{
  uint8_t data[BUFF_SIZE];
  uint8_t head;             // pozycja nastêpnego wolnego bajtu
  uint8_t tail;             // ostatni odczytany bajt
}TCircBuff;

volatile TCircBuff usart2_tx_buff;
volatile TCircBuff usart2_rx_buff;

typedef struct
{
  USART_TypeDef * usart;
  GPIO_TypeDef * tx_port;
  GPIO_TypeDef * rx_port;
  uint8_t tx_pin;
  uint8_t rx_pin;
  uint32_t tx_pin_cfg;
  uint32_t rx_pin_cfg;
  uint32_t clk;
  uint32_t baud_rate;
  volatile uint32_t * clk_en_reg;
  uint32_t clk_en_val;
  volatile TCircBuff * tx_buff;
  volatile TCircBuff * rx_buff;
  IRQn_Type IRqn;
}TSerial;

void ConfigureSerialPorts(void);
void SerialSendByte(USART_TypeDef * usart_id, const char data);
void SerialSendString(USART_TypeDef * usart_id, const char * str);

#endif /* SERIAL_H_ */
