/**
  ******************************************************************************
  * @file           : flash_rw.cpp
  * @author         : Rusanov M.N.
  ******************************************************************************
  */

#include "flash_rw.h"
#include <cstring>

flash_rw::flash_rw()
  : flash_t() {}

void flash_rw::erase(const uint32_t address_start, const uint32_t address_end)
{
  HAL_FLASH_Unlock();

  FLASH_EraseInitTypeDef erase_init_struct;
  init_erase_struct(erase_init_struct, address_start, address_end);

  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  
  if (uint32_t index_of_sector_error = 0; 
      HAL_FLASHEx_Erase(&erase_init_struct, &index_of_sector_error) != HAL_OK)
  {
    /*
      Error occurred while sector erase.
      User can add here some code to deal with this error.
      SECTORError will contain the faulty sector and then to know the code error on this sector,
      user can call function 'HAL_FLASH_GetError()'
    */
    Error_Handler();
  }

  HAL_FLASH_Lock();
}

void flash_rw::write_word(const uint32_t address, const uint32_t word)
{
  if (const HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, word);
      status != HAL_OK) 
  {
    /* Error occurred while writing data in Flash memory.
       User can add here some code to deal with this error */
    Error_Handler();
  }
}

void flash_rw::write_only_one_word(const uint32_t address, const uint32_t word)
{
  HAL_FLASH_Unlock();
  write_word(address, word);
  HAL_FLASH_Lock();
}

uint32_t flash_rw::read_word(const uint32_t address)
{
  return *reinterpret_cast<__IO uint32_t*>(address);  // NOLINT(performance-no-int-to-ptr)
}

void flash_rw::check_word(const uint32_t address, const uint32_t word)
{
  if (read_word(address) != word)
  {
    Error_Handler();
  }
}

void flash_rw::write_arr_to_flash(const uint32_t address, const uint32_t* arr, const size_t size_bytes)
{
  if (const uint32_t end_addr = address + size_bytes - 1;
      end_addr > max_end_addr)
  {
    Error_Handler();
  }

  HAL_FLASH_Unlock();

  const size_t whole_words = size_bytes / sizeof(uint32_t);
  for (size_t i = 0; i < whole_words; ++i)
  {
    const size_t addr = address + i * sizeof(uint32_t);
    write_word(addr, arr[i]);
    check_word(addr, arr[i]);
  }

  if (const size_t remaining_bytes = size_bytes % sizeof(uint32_t);
    remaining_bytes > 0)
  {
    const uint32_t mask = (remaining_bytes == 1)
                                 ? 0x000000FF
                                 : (remaining_bytes == 2)
                                     ? 0x0000FFFF
                                     : 0x00FFFFFF;
    const uint32_t last_word = arr[whole_words] & mask;
    const size_t addr = address + size_bytes - remaining_bytes;
    write_word(addr, last_word);
    check_word(addr, last_word);
  }

  HAL_FLASH_Lock();
}

void flash_rw::read_arr_from_flash(const uint32_t address, uint32_t* arr, const size_t size_bytes)
{
  if (const uint32_t end_addr = address + size_bytes - 1;
      end_addr > max_end_addr)
  {
    Error_Handler();
  }

  const size_t whole_words = size_bytes / sizeof(uint32_t);
  for (size_t i = 0; i < whole_words; ++i)
  {
    arr[i] = read_word(address + i * sizeof(uint32_t));
  }

  if (const size_t remaining_bytes = size_bytes % sizeof(uint32_t);
    remaining_bytes > 0)
  {
    const size_t addr = address + size_bytes - remaining_bytes;
    const uint32_t mask = (remaining_bytes == 1)
                                 ? 0x000000FF
                                 : (remaining_bytes == 2)
                                     ? 0x0000FFFF
                                     : 0x00FFFFFF;
    arr[whole_words] = (arr[whole_words] & ~mask) | (read_word(addr) & mask);
  }
}

void flash_rw::write_str_to_flash(const uint32_t address, const char* str)
{
  write_arr_to_flash(address, reinterpret_cast<const uint32_t*>(str), strlen(str) + 1);
}

size_t flash_rw::read_str_from_flash(uint32_t address, char* str, const size_t max_size)
{
  size_t size = 0;
  uint32_t end_addr = address + max_size - 1;

  if (end_addr >= max_end_addr)
  {
    end_addr = max_end_addr;
  }

  while (address <= end_addr)
  {
    const uint32_t word = read_word(address);

    for (size_t i = 0; i < sizeof(uint32_t); ++i)
    {
      const char c = static_cast<char>((word >> (i * 8)) & 0xFF);
      str[size++] = c;

      if (c == '\0')
      {
        return size;
      }

      if (size >= max_size)
      {
        return size;
      }
    }

    address += sizeof(uint32_t);
  }
  return size;
}
