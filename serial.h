/*
 * serial.h
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#define SIZE 128

typedef struct
{
  uint8_t buff[SIZE];
  uint8_t head;
  uint8_t tail;
}TCircBuff;

typedef struct
{
  USART_TypeDef * usart_id;   // USART number
  uint32_t baud_rate;          // baud rate
  uint32_t clk;               // entry USART clock frequency
  TCircBuff * read_buff;      // buffer for read
  TCircBuff * write_buff;     // buffer for write
}TSerialPort;

void serial_execute_config(TSerialPort * port);
void SerialPortConfigure(USART_TypeDef * usart_id, uint32_t baud_rate, uint32_t clk, TCircBuff * read_buff, TCircBuff * write_buff);

#endif /* SERIAL_H_ */
