/*
 * serial.c
 *
 *  Created on: 2 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

/* stm32L4 disco - komunikacja RS232, VCOM PORT */
static const TSerial usart2_cfg = {USART2, USART2_TX_PORT, USART2_RX_PORT, USART2_TX_PIN, USART2_RX_PIN, USART2_TX_PIN_CFG, USART2_RX_PIN_CFG, USART2_CLK, USART2_BR, USART2_CLK_EN_REG, USART2_CLK_EN_VAL, &usart2_tx_buff, &usart2_rx_buff, USART2_IRQn};

/* USART wykorzystywany do komunikacji interfejsem 1-wire, wykomentowany -- poki co robie na timerze*/
//static const TSerial usart3_cfg = {USART3, USART3_TX_PORT, USART3_RX_PORT, USART3_TX_PIN, USART3_RX_PIN, USART3_TX_PIN_CFG, USART3_RX_PIN_CFG, USART3_CLK, USART3_BR, USART3_CLK_EN_REG, USART3_CLK_EN_VAL, 0, 0, USART3_IRQn};

static const TSerial * const serial_interfaces[] =
{
    &usart2_cfg,
//    &usart2_cfg
};

// rowinac makro przy kolejnych serialach
#define _GetSerialPtr(usart_id) (usart_id == USART2 ? (const TSerial *)&usart2_cfg : (void *)0)

void SerialInit(void)
{
  uint8_t i;

  /* configure every defined USART */
  for (i = 0; i < _NumOfArrayMemb(serial_interfaces); i++)
  {
    /* configure ports, pins */
    gpio_pin_cfg(serial_interfaces[i]->tx_port, serial_interfaces[i]->tx_pin, serial_interfaces[i]->tx_pin_cfg);
    gpio_pin_cfg(serial_interfaces[i]->rx_port, serial_interfaces[i]->rx_pin, serial_interfaces[i]->rx_pin_cfg);

    /* turn on clock */
    *serial_interfaces[i]->clk_en_reg |= serial_interfaces[i]->clk_en_val;

    /* set baud rate */
    serial_interfaces[i]->usart->BRR = serial_interfaces[i]->clk / serial_interfaces[i]->baud_rate;

    /* enable */
    serial_interfaces[i]->usart->CR1 =  USART_CR1_UE      |   // usart enable
                                        USART_CR1_TE      |   // transmitter enable
                                        USART_CR1_RE      |   // receiver enable
                                        USART_CR1_RXNEIE;     // receiver not empty interrupt enable

    /* turn on interrupts */
    NVIC_EnableIRQ(serial_interfaces[i]->IRqn);
  }
}

void SerialSendByte(USART_TypeDef * usart_id, const char data)
{
  uint8_t tmp_head;
  const TSerial * si;

  si = _GetSerialPtr(usart_id);

  tmp_head = (si->tx_buff->head + 1) & BUFF_MASK;

  /* czekaj na miejsce w buforze */
  while (tmp_head == si->tx_buff->tail);

  si->tx_buff->data[tmp_head] = data;
  si->tx_buff->head = tmp_head;
  si->usart->CR1 |= USART_CR1_TXEIE;
}

void SerialSendString(USART_TypeDef * usart_id, const char * str)
{
  uint8_t tmp_head;
  const TSerial * si;

  si = _GetSerialPtr(usart_id);

  while (*str)
  {
    tmp_head = (si->tx_buff->head + 1) & BUFF_MASK;
    while (tmp_head == si->tx_buff->tail);
    si->tx_buff->data[tmp_head] = *str++;
    si->tx_buff->head = tmp_head;
  }
  si->usart->CR1 |= USART_CR1_TXEIE;
}

// TODO zdeklarowac wskaznik do usart2_tx_buff i nim sie poslugiwac
//      zdeklarowac timer i porownac czas trwania tego przerwania
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
