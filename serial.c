/*
 * serial.c
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

IRQn_Type ConvUsartTypeToIrqnType(USART_TypeDef * usart_id)
{
  if (usart_id == USART1)
    return USART1_IRQn;
  if (usart_id == USART2)
    return USART2_IRQn;
  if (usart_id == USART3)
    return USART3_IRQn;
  else
    return 0;
}

void SerialPortConfigure(USART_TypeDef * usart_id, uint32_t baud_rate)
{
  if (!IS_USART_INSTANCE(usart_id))  // needed ?
    return;

  RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

  /* set BR */
  usart_id->BRR = APB2_CLK / baud_rate;

  usart_id->CR1 = USART_CR1_UE      |   // usart enable
                  USART_CR1_TE;//      |   // transmitter enable
//                  USART_CR1_RE      |   // receiver enable
//                  USART_CR1_RXNEIE;     // receiver not empty interrupt enable

  NVIC_EnableIRQ(ConvUsartTypeToIrqnType(usart_id));
}

void SerialSendByte(USART_TypeDef * usart_id, volatile TCircBuff * buff, uint8_t data)
{
  uint8_t tmp_head;

  tmp_head = (buff->head + 1) & BUFF_MASK;

  /* czekaj na miejsce w buforze */
  while (tmp_head == buff->tail);

  buff->data[tmp_head] = data;
  buff->head = tmp_head;
  usart_id->CR1 |= USART_CR1_TXEIE;
}

void USART2_IRQHandler(void)
{
  uint8_t tmp_tail;

  /* TX irqn */
  if((USART2->ISR & USART_ISR_TXE) && (USART2->CR1 & USART_CR1_TXEIE))
  {
    tmp_tail = usart2_tx_buff.tail;
    if (tmp_tail != usart2_tx_buff.head)
    {
      tmp_tail = (tmp_tail + 1) & BUFF_MASK;
      USART2->TDR = usart2_tx_buff.data[tmp_tail];
      usart2_tx_buff.tail = tmp_tail;
    }
    else
      USART2->CR1 &= ~(USART_CR1_TXEIE);
  }
}
