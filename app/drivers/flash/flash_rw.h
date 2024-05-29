/**
  ******************************************************************************
  * @file           : flash_rw.h
  * @author         : Rusanov M.N.
  * @version        : V1.0.1
  * @date           : 24-May-2024
  * @brief          : Header for flash_rw.cpp file.
  *                   This file contains functions for working with flash memory
  *
  ******************************************************************************
  */

#pragma once

#include "main.h"
#if defined(STM32F746xx)
#include "flash_f7.h"
using flash_t = flash_f7;
#elif defined(STM32L152xC)
#include "flash_l1.h"
using flash_t = flash_l1;
#endif

class flash_rw : public flash_t
{
protected:
  flash_rw();
  static void erase(uint32_t address_start, uint32_t address_end); // Use before write!
  static void write_only_one_word(uint32_t address, uint32_t word);
  [[nodiscard]] static uint32_t read_word(uint32_t address);
  static void write_arr_to_flash(uint32_t address, const uint32_t* arr, size_t size_bytes);
  static void read_arr_from_flash(uint32_t address, uint32_t* arr, size_t size_bytes);
  static void write_str_to_flash(uint32_t address, const char* str);
  static size_t read_str_from_flash(uint32_t address, char* str, size_t max_size);

private:
  static void write_word(uint32_t address, uint32_t word);
  static void check_word(uint32_t address, uint32_t word);
};


