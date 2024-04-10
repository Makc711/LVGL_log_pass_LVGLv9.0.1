/*********************
*      INCLUDES
*********************/
#include "memory.h"
#ifdef EMBEDDED
#include <cstring>
#include "flash_rw.h"
#else
#include <iostream>
#include <fstream>
#endif

/*********************
 *     CONSTANTS
 *********************/
#ifdef EMBEDDED
constexpr auto k_flash_data_start_addr = ADDR_FLASH_SECTOR_7;
constexpr auto k_flash_data_end_addr = k_flash_data_start_addr + k_username_max_length + k_password_max_length;
constexpr auto k_username_addr = k_flash_data_start_addr;
constexpr auto k_password_addr = k_username_addr + k_username_max_length;
#else
constexpr auto k_filename = "log_pass.txt";
#endif

/**********************
 *  STATIC PROTOTYPES
 **********************/
#ifdef EMBEDDED
static void first_write_log_pass_to_flash();
#else
static void first_write_log_pass_to_file();
static void write_log_pass_to_file(const char* filename, const char* username, const char* password);
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void memory_read_log_pass(char* username, char* password)
{
#ifdef EMBEDDED
  first_write_log_pass_to_flash();
  read_str_from_flash(k_username_addr, username, k_username_max_length);
  read_str_from_flash(k_password_addr, password, k_password_max_length);

#else
  first_write_log_pass_to_file();

  if (std::ifstream read_file(k_filename); read_file.is_open())
  {
    read_file.getline(username, k_username_max_length);
    read_file.getline(password, k_password_max_length);
  }
  else
  {
    strcpy(username, k_default_username);
    strcpy(password, k_default_password);
  }
#endif
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
#ifdef EMBEDDED
static void first_write_log_pass_to_flash()
{
  char read_username[k_username_max_length];
  read_str_from_flash(k_username_addr, read_username, k_username_max_length);

  char read_password[k_password_max_length];
  read_str_from_flash(k_password_addr, read_password, k_password_max_length);

  if ((strcmp(k_default_username, read_username) != 0) || (strcmp(k_default_password, read_password) != 0))
  {
    erase_sector(k_flash_data_start_addr, k_flash_data_end_addr);
    write_str_to_flash(k_default_username, k_username_addr);
    write_str_to_flash(k_default_password, k_password_addr);
  }
}
#else

static void first_write_log_pass_to_file()
{
  if (std::ifstream read_file(k_filename); read_file.is_open())
  {
    std::string read_username;
    std::string read_password;
    read_file >> read_username;
    read_file >> read_password;
    read_file.close();

    if (read_username.empty() || (read_password.length() < k_password_min_length))
    {
      write_log_pass_to_file(k_filename, k_default_username, k_default_password);
    }
  }
  else
  {
    write_log_pass_to_file(k_filename, k_default_username, k_default_password);
  }
}

static void write_log_pass_to_file(const char* filename, const char* username, const char* password)
{
  if (std::ofstream file(filename); file.is_open())
  {
    file << username << std::endl;
    file << password << std::endl;
    file.close();
  }
  else
  {
    std::cerr << "Failed to open the file for writing." << std::endl;
  }
}
#endif

