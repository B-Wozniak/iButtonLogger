/*
 * serial.c
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

/* STM32L476VG specific */
// bond USARTX with APBX

/*
 * @brief basic settings for choosen USARTX
 * @param usart_id usart macro from stm libraries eg. USART1, USART2
 * @param baud_rate bad rate to set
 * @param TCircBuff circulal
 */
void SerialPortConfigure(USART_TypeDef * usart_id, uint32_t baud_rate, uint32_t clk, TCircBuff * read_buff, TCircBuff * write_buff)
{
  TSerialPort serial_port;

  if (IS_USART_INSTANCE(usart_id))  // needed ?
    serial_port.usart_id = usart_id;

  /* set baud rate */
  serial_port.baud_rate = baud_rate;

  /* TODO check conditions for valid clock; distinguish the usart's (all on the same bus ?) */
  serial_port.clk = clk;

  /* assign data buffers */
  serial_port.read_buff = read_buff;
  serial_port.write_buff = write_buff;

  serial_execute_config(&serial_port);
}

void serial_execute_config(TSerialPort * port)
{
  /* basic configuration with interrupts */
  port->usart_id->CR2 = USART_CR1_UE      |   // usart enable
                        USART_CR1_TE      |   // transmitter enable
                        USART_CR1_RE      |   // receiver enable
                        USART_CR1_RXNEIE;     // receiver not empty interrupt enable

  /* turn on interrupts for this USART */
//  NVIC_EnableIRQ(_concat_IRQn(port->usart_id)); // zle ! konkatenacja po preprocu ! to nie bedzie dzialac ! FIXME !
}
