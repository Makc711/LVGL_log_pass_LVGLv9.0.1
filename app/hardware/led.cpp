/**
  ******************************************************************************
  * @file           : led.cpp
  * @author         : Rusanov M.N.
  ******************************************************************************
  */

#include "led.h"

led::led(GPIO_TypeDef* gpio_port, const uint16_t gpio_pin, const conn connection)
  : f_gpio_port(gpio_port)
  , f_gpio_pin(gpio_pin)
  , f_connection(connection) {}

void led::on() const
{
  HAL_GPIO_WritePin(f_gpio_port, f_gpio_pin, static_cast<GPIO_PinState>(f_connection));
}

void led::off() const
{
  const auto pin_state = static_cast<GPIO_PinState>((f_connection == conn::FORWARD) 
                        ? conn::REVERSE
                        : conn::FORWARD);
  HAL_GPIO_WritePin(f_gpio_port, f_gpio_pin, pin_state);
}

void led::toggle() const
{
  HAL_GPIO_TogglePin(f_gpio_port, f_gpio_pin);
}
