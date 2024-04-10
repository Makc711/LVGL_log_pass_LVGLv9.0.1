/*********************
*      INCLUDES
*********************/
#include "mvc.h"
#include <cstring>
#include "auth_model.h"
#include "auth_controller.h"
#ifdef EMBEDDED
#include "flash_rw.h"
#else
#include <iostream>
#include <fstream>
#endif

/*********************
 *      DEFINES
 *********************/
#define DEFAULT_USERNAME ("Maxim")
#define DEFAULT_PASSWORD ("123456")
#define USERNAME_MAX_LENGTH    20
#define PASSWORD_MAX_LENGTH    20

#ifdef EMBEDDED
#define FLASH_DATA_START_ADDR  ADDR_FLASH_SECTOR_7
#define FLASH_DATA_END_ADDR    (FLASH_DATA_START_ADDR + USERNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH)
#define USERNAME_ADDR          FLASH_DATA_START_ADDR
#define PASSWORD_ADDR          (USERNAME_ADDR + USERNAME_MAX_LENGTH)
#else
#define FILENAME               ("log_pass.txt")
#define MIN_PASS_LENGTH        6
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
static auth_model* g_model;
static auth_controller* g_auth_controller;

/**********************
 *  STATIC PROTOTYPES
 **********************/
#ifdef EMBEDDED
static void first_write_log_pass_to_flash(void);
#else
static void first_write_log_pass_to_file(void);
static void write_log_pass_to_file(const std::string& filename, const std::string& username, const std::string& password);
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void mvc_init(void)
{
  const auto read_username = new char[USERNAME_MAX_LENGTH];
  const auto read_password = new char[PASSWORD_MAX_LENGTH];

#ifdef EMBEDDED
  first_write_log_pass_to_flash();
  read_str_from_flash(USERNAME_ADDR, read_username, USERNAME_MAX_LENGTH);
  read_str_from_flash(PASSWORD_ADDR, read_password, PASSWORD_MAX_LENGTH);

#else
  first_write_log_pass_to_file();

  if (std::ifstream read_file(FILENAME); read_file.is_open())
  {
    read_file >> read_username;
    read_file >> read_password;
    read_file.close();
  }
  else
  {
    strcpy(read_username, DEFAULT_USERNAME);
    strcpy(read_password, DEFAULT_PASSWORD);
  }
#endif

  g_model = new auth_model(read_username, read_password);
  g_auth_controller = new auth_controller(g_model);
}

void mvc_deinit(void)
{
  delete g_auth_controller;
}

void mvc(const char* username, const char* password)
{
  g_auth_controller->authenticate_user(username, password);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
#ifdef EMBEDDED
static void first_write_log_pass_to_flash(void)
{
  char read_username[USERNAME_MAX_LENGTH];
  read_str_from_flash(USERNAME_ADDR, read_username, USERNAME_MAX_LENGTH);

  char read_password[USERNAME_MAX_LENGTH];
  read_str_from_flash(PASSWORD_ADDR, read_password, PASSWORD_MAX_LENGTH);

  if ((strcmp(DEFAULT_USERNAME, read_username) != 0) || (strcmp(DEFAULT_PASSWORD, read_password) != 0))
  {
    erase_sector(FLASH_DATA_START_ADDR, FLASH_DATA_END_ADDR);
    write_str_to_flash(DEFAULT_USERNAME, USERNAME_ADDR);
    write_str_to_flash(DEFAULT_PASSWORD, PASSWORD_ADDR);
  }
}
#else

static void first_write_log_pass_to_file(void)
{
  if (std::ifstream read_file(FILENAME); read_file.is_open())
  {
    std::string read_username;
    std::string read_password;
    read_file >> read_username;
    read_file >> read_password;
    read_file.close();

    if (read_username.empty() || (read_password.length() < MIN_PASS_LENGTH))
    {
      write_log_pass_to_file(FILENAME, DEFAULT_USERNAME, DEFAULT_PASSWORD);
    }
  }
  else
  {
    write_log_pass_to_file(FILENAME, DEFAULT_USERNAME, DEFAULT_PASSWORD);
  }
}

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

