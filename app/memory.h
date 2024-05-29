/**
  ******************************************************************************
  * @file           : memory.h
  * @author         : Rusanov M.N.
  * @version        : V2.0.0
  * @date           : 29-May-2024
  * @brief          : Header for memory.cpp file.
  *                   This file contains functions for working with internal
  *                   STM32 memory
  *
  ******************************************************************************
  */

#pragma once

#ifdef EMBEDDED
#include "main.h"
#include "flash_rw.h"
#else
#include <cstdint>
#endif

constexpr uint32_t up_to_32_bit(const uint32_t value)
{
  return (value + sizeof(uint32_t) - 1) & ~(sizeof(uint32_t) - 1);
}

constexpr auto k_username_max_length = up_to_32_bit(20);
constexpr auto k_password_max_length = up_to_32_bit(20);
constexpr auto k_password_min_length = 6;

struct data_t
{
  char username[k_username_max_length];
  char password[k_password_max_length];
};

constexpr data_t k_default_val = {
  "Maxim",
  "123456"
};

#ifdef EMBEDDED
class memory : public flash_rw
#else
class memory
#endif
{
public:
  enum class status
  {
    READ_OK,
    GET_DEFAULT
  };

#ifdef EMBEDDED
  explicit memory(addr start_addr, CRC_HandleTypeDef& hcrc);
#else
  explicit memory(const char* filename);
#endif
  status read_data(data_t& data) const;

private:
#ifdef EMBEDDED
  const uint32_t f_data_start_addr;
  const uint32_t f_data_end_addr;
  CRC_HandleTypeDef &f_hcrc;
#else
  void write_data_to_file(const data_t& data) const;
  void read_data_from_file(data_t& data) const;

  const char* f_filename;
#endif
};