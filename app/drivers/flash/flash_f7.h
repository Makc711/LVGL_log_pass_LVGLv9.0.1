/**
  ******************************************************************************
  * @file           : flash_f7.h
  * @author         : Rusanov M.N.
  * @version        : V1.0.0
  * @date           : 20-May-2024
  * @brief          : Header for flash_f7.cpp file.
  *                   This file contains function for erase flash memory
  *
  ******************************************************************************
  */

#pragma once

#include "main.h"

class flash_f7
{
public:
  enum class addr
  {
    ADDR_FLASH_SECTOR_0 = static_cast<uint32_t>(0x08000000), /* Base address of Sector 0, 32 Kbytes */
    ADDR_FLASH_SECTOR_1 = static_cast<uint32_t>(0x08008000), /* Base address of Sector 1, 32 Kbytes */
    ADDR_FLASH_SECTOR_2 = static_cast<uint32_t>(0x08010000), /* Base address of Sector 2, 32 Kbytes */
    ADDR_FLASH_SECTOR_3 = static_cast<uint32_t>(0x08018000), /* Base address of Sector 3, 32 Kbytes */
    ADDR_FLASH_SECTOR_4 = static_cast<uint32_t>(0x08020000), /* Base address of Sector 4, 128 Kbytes */
    ADDR_FLASH_SECTOR_5 = static_cast<uint32_t>(0x08040000), /* Base address of Sector 5, 256 Kbytes */
    ADDR_FLASH_SECTOR_6 = static_cast<uint32_t>(0x08080000), /* Base address of Sector 6, 256 Kbytes */
    ADDR_FLASH_SECTOR_7 = static_cast<uint32_t>(0x080C0000)  /* Base address of Sector 7, 256 Kbytes */
  };

  static constexpr uint32_t max_end_addr = static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_7) + 256 * 1024 - 1;

protected:
  flash_f7();
  static void init_erase_struct(
    FLASH_EraseInitTypeDef &erase_init_struct, 
    uint32_t address_start, 
    uint32_t address_end);

private:
  static uint32_t get_sector(uint32_t address);
};