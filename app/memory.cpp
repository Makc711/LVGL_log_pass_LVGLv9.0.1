/*********************
*      INCLUDES
*********************/
#include "memory.h"
#ifdef EMBEDDED
#include "flash_rw.h"
#else
#include <iostream>
#include <fstream>
#endif

#ifdef EMBEDDED

memory::memory(const addr start_addr, CRC_HandleTypeDef& hcrc) :
  flash_rw(),
  f_data_start_addr(static_cast<uint32_t>(start_addr)),
  f_data_end_addr(f_data_start_addr + sizeof(k_default_val)),
  f_hcrc(hcrc) {}

memory::status memory::read_data(data_t& data) const
{
  read_arr_from_flash(f_data_start_addr, reinterpret_cast<uint32_t*>(&data), sizeof(k_default_val));
  const uint32_t crc_read = read_word(f_data_end_addr);

  if (const uint32_t crc_calc = HAL_CRC_Calculate(&f_hcrc, 
      reinterpret_cast<uint32_t*>(&data), 
      sizeof(k_default_val) / sizeof(uint32_t)); 
      crc_read != crc_calc)
  {
    erase(f_data_start_addr, f_data_end_addr);
    write_arr_to_flash(f_data_start_addr, reinterpret_cast<const uint32_t*>(&k_default_val), sizeof(k_default_val));
    read_arr_from_flash(f_data_start_addr, reinterpret_cast<uint32_t*>(&data), sizeof(k_default_val));
    const uint32_t crc = HAL_CRC_Calculate(&f_hcrc, 
      reinterpret_cast<uint32_t*>(&data), 
      (k_username_max_length + k_password_max_length) / sizeof(uint32_t)); 
    write_only_one_word(f_data_end_addr, crc);
    return status::GET_DEFAULT;
  }
  return status::READ_OK;
}

#else

memory::memory(const char* filename):
  f_filename(filename) {}

memory::status memory::read_data(data_t& data) const
{
  read_data_from_file(data);
  if ((strlen(data.username) == 0) || (strlen(data.password) < k_password_min_length))
  {
    write_data_to_file(k_default_val);
    read_data_from_file(data);
    return status::GET_DEFAULT;
  }
  return status::READ_OK;
}

void memory::write_data_to_file(const data_t& data) const
{
  if (std::ofstream file(f_filename); file.is_open())
  {
    file << data.username << std::endl;
    file << data.password << std::endl;
    file.close();
  }
  else
  {
    std::cerr << "Failed to open the file for writing." << std::endl;
  }
}

void memory::read_data_from_file(data_t& data) const
{
  if (std::ifstream read_file(f_filename); read_file.is_open())
  {
    read_file >> data.username;
    read_file >> data.password;
    read_file.close();
  }
  else
  {
    std::cerr << "Failed to open the file for reading." << std::endl;
  }
}

#endif
