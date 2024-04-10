/*********************
*      INCLUDES
*********************/
#include "mvc.h"
#include <string>
#include "auth_model.h"
#include "auth_view.h"
#include "auth_controller.h"
#include "gui/gui_app.h"
#ifdef EMBEDDED
#include "flash_rw.h"
#else
#include <iostream>
#include <fstream>
#endif

/**********************
 *     CONSTANTS
 **********************/
const std::string k_default_username = "Maxim";
const std::string k_default_password = "123456";
#ifdef EMBEDDED
constexpr uint32_t k_flash_data_start_addr = ADDR_FLASH_SECTOR_7;
#else
constexpr const char* k_filename = "log_pass.txt";
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
static auth_model* g_model;
static auth_view* g_view;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool is_initialized(const char* username, const char* password);
#ifdef EMBEDDED
static void write_log_pass_to_flash(const std::string& filename, const std::string& username, const std::string& password);
#else
static void write_log_pass_to_file(const std::string& filename, const std::string& username, const std::string& password);
#endif


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void mvc_init(void)
{
  std::string username = k_default_username;
  std::string password = k_default_password;

#ifdef EMBEDDED
  const uint32_t max_data_size = username.length() + password.length() + 2;
  const uint32_t flash_data_end_addr = k_flash_data_start_addr + max_data_size;
  const uint32_t username_addr = k_flash_data_start_addr;
  const uint32_t password_addr = username_addr + username.length() + 1;

  auto read_username = new char[username.length()];
  read_str_from_flash(username_addr, read_username, max_data_size);

  auto read_password = new char[password.length()];
  read_str_from_flash(password_addr, read_password, max_data_size);

  if ((read_username != username) || (read_password != password))
  {
    erase_sector(k_flash_data_start_addr, flash_data_end_addr);
    write_str_to_flash(username.c_str(), username_addr);
    write_str_to_flash(password.c_str(), password_addr);
  }
  else
  {
    username = read_username;
    password = read_password;
  }

  delete[] read_username;
  delete[] read_password;

#else
  if (std::ifstream read_file(k_filename); read_file.is_open())
  {
    std::string read_username;
    std::string read_password;
    read_file >> read_username;
    read_file >> read_password;
    read_file.close();

    if ((read_username.length() == 0) || (read_password.length() < 6))
    {
      write_log_pass_to_file(k_filename, username, password);
    }
    else
    {
      username = read_username;
      password = read_password;
    }
  }
  else
  {
    write_log_pass_to_file(k_filename, username, password);
  }
#endif

  g_model = new auth_model(username, password);
  g_view = new auth_view(get_text_message_label());
}

void mvc(const char* username, const char* password)
{
  if (!is_initialized(username, password))
  {
    // TODO: Error handling: variables not initialized
    return;
  }

  static std::string prev_entered_username;
  static std::string prev_entered_password;

  if ((prev_entered_username != username) || (prev_entered_password != password))
  {
    const auth_controller auth_controller(g_model, g_view);
    auth_controller.authenticate_user(username, password);
    prev_entered_username = username;
    prev_entered_password = password;
  }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static bool is_initialized(const char* username, const char* password)
{
  if (!username || !password || !g_model || !g_view)
  {
    return false;
  }
  return true;
}

#ifdef EMBEDDED
static void write_log_pass_to_flash(const std::string& filename, const std::string& username, const std::string& password)
{
  
}

#else

static void write_log_pass_to_file(const std::string& filename, const std::string& username, const std::string& password)
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

