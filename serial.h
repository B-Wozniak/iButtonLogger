/*
 * serial.h
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#define USART2_TX_PORT GPIOD
#define USART2_RX_PORT GPIOD

#define USART2_TX_PIN 5
#define USART2_RX_PIN 6

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
  volatile TCircBuff * tx_buff;
  volatile TCircBuff * rx_buff;
  IRQn_Type IRqn;
}TSerial;

IRQn_Type ConvUsartTypeToIrqnType(USART_TypeDef * usart_id);
void SerialPortConfigure(USART_TypeDef * usart_id, uint32_t baud_rate);
void SerialSendByte(USART_TypeDef * usart_id, volatile TCircBuff * buff, uint8_t data);

#endif /* SERIAL_H_ */
