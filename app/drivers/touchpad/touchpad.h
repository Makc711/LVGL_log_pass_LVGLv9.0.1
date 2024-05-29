/**
  ******************************************************************************
  * @file           : touchpad.h
  * @author         : Rusanov M.N.
  * @version        : V2.0.0
  * @date           : 29-May-2024
  * @brief          : Header for touchpad.cpp file.
  *                   This file contains functions for working with touchpad
  *                   STM32F746G-DISCOVERY for LVGL.
  *
  ******************************************************************************
  */

#pragma once

#include "lvgl/lvgl.h"
#include "led.h"

class touchpad
{
public:
  static void init();

private:
  static void read(lv_indev_t* indev, lv_indev_data_t* data);

private:
  static led f_led_green;
};
