/**
  ******************************************************************************
  * @file           : led.h
  * @author         : Rusanov M.N.
  * @version        : V1.0.0
  * @date           : 13-May-2024
  * @brief          : Header for led.cpp file.
  *                   This file contains functions for working with LEDs.
  *
  ******************************************************************************
  */

#pragma once

#include "main.h"

class led final
{
public:
  enum class conn
  {
    FORWARD = GPIO_PIN_SET,
    REVERSE = GPIO_PIN_RESET
  };

  explicit led(GPIO_TypeDef* gpio_port, uint16_t gpio_pin, conn connection);
  void on() const;
  void off() const;
  void toggle() const;

private:
  GPIO_TypeDef* f_gpio_port;
  uint16_t f_gpio_pin;
  conn f_connection;
};

