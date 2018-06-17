/*
 * gpio.c
 *
 *  Created on: 17 cze 2018
 *      Author: Bart
 */

#include "iButtonLogger.h"

void GpioConfig(void)
{
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIODEN |
                   RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOFEN | RCC_AHB2ENR_GPIOGEN | RCC_AHB2ENR_GPIOHEN);

  gpio_pin_cfg(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_OUT_PP_25MHz);
  gpio_pin_cfg(USART2_TX_PORT, USART2_TX_PIN, GPIO_AF7_PP_2MHz);
}

/*
 * @brief gpio_pin_cfg
 * author Freddie Chopin, http://www.freddiechopin.info/
 */
void gpio_pin_cfg(GPIO_TypeDef *port_ptr, uint32_t pin, uint32_t configuration)
{
  uint32_t moder, otyper, ospeedr, pupdr, afr, afrx;

  moder = port_ptr->MODER;        // localize
  moder &= ~(GPIO_MODER_mask << (pin * 2)); // clear current setting
  moder |= (GPIO_GET_MODER(configuration) << (pin * 2));  // apply new setting
  port_ptr->MODER = moder;        // save back

  otyper = port_ptr->OTYPER;        // localize
  otyper &= ~(GPIO_OTYPER_mask << pin); // clear current setting
  otyper |= (GPIO_GET_OTYPER(configuration) << pin);  // apply new setting
  port_ptr->OTYPER = otyper;        // save back

  ospeedr = port_ptr->OSPEEDR;        // localize
  ospeedr &= ~(GPIO_OSPEEDR_mask << (pin * 2)); // clear current setting
  ospeedr |= (GPIO_GET_OSPEEDR(configuration) << (pin * 2));  // apply new setting
  port_ptr->OSPEEDR = ospeedr;        // save back

  pupdr = port_ptr->PUPDR;        // localize
  pupdr &= ~(GPIO_PUPDR_mask << (pin * 2)); // clear current setting
  pupdr |= (GPIO_GET_PUPDR(configuration) << (pin * 2));  // apply new setting
  port_ptr->PUPDR = pupdr;        // save back

  if (pin < 8)              // determine which AFR register should be used
    afrx = 0;             // AFRL - pins [0; 7]
  else
  {
    afrx = 1;             // AFRH - pins [8; 15]
    pin -= 8;             // trim pin number
  }

  afr = port_ptr->AFR[afrx];        // localize
  afr &= ~(GPIO_AFRx_mask << (pin * 4));  // clear current setting
  afr |= (GPIO_GET_AFR(configuration) << (pin * 4));  // apply new setting
  port_ptr->AFR[afrx] = afr;        // save back
}
