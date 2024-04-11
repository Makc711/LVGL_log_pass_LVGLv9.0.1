/**
 * @file app.cpp
 *
 */

 /*********************
 *      INCLUDES
 *********************/
#include "app.h"
#include "app_tasks.h"
#include "lvgl.h"
#ifdef EMBEDDED
#include "memory.h"
#include "hardware.h"
#include "tft.h"
#include "touchpad.h"
#else
#include <Windows.h>
#include <LvglWindowsIconResource.h>
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
#ifndef EMBEDDED
lv_display_t* g_display;
#endif

/**********************
 *  STATIC PROTOTYPES
 **********************/
static error_code_t display_init();
static error_code_t input_device_init();
#ifndef EMBEDDED
static error_code_t windows_display_init();
static error_code_t windows_input_device_init();
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
error_code_t app_main()
{
#ifdef EMBEDDED
  hardware_init();
#endif

  lv_init();

  if (const error_code_t res = display_init(); 
    res != error_code_t::OK)
  {
    return res;
  }

  if (const error_code_t res = input_device_init(); 
    res != error_code_t::OK)
  {
    return res;
  }

  app_create_tasks();

  return error_code_t::OK;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static error_code_t display_init()
{
#ifdef EMBEDDED
  tft_init();
#else
  if (const error_code_t res = windows_display_init();
    res != error_code_t::OK)
  {
    return res;
  }
#endif

  return error_code_t::OK;
}

static error_code_t input_device_init()
{
#ifdef EMBEDDED
  touchpad_init();
#else
  if (const error_code_t res = windows_input_device_init();
    res != error_code_t::OK)
  {
    return res;
  }
#endif

  return error_code_t::OK;
}

#ifndef EMBEDDED
static error_code_t windows_display_init()
{
#if LV_TXT_ENC == LV_TXT_ENC_UTF8
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
#endif

  g_display = lv_windows_create_display(
      L"LVGL Windows Simulator Display",
      480,
      272,
      100,
      false,
      true);
  if (!g_display)
  {
    return error_code_t::ERROR_CREATE_DISPLAY;
  }

  HWND window_handle = lv_windows_get_display_window_handle(g_display);
  if (!window_handle)
  {
    return error_code_t::ERROR_GET_DISPLAY_HANDLE;
  }

  if (HICON icon_handle = LoadIconW(
    GetModuleHandleW(nullptr),
    MAKEINTRESOURCE(IDI_LVGL_WINDOWS)))
  {
    SendMessageW(
        window_handle,
        WM_SETICON,
        TRUE,
        reinterpret_cast<LPARAM>(icon_handle));
    SendMessageW(
        window_handle,
        WM_SETICON,
        FALSE,
        reinterpret_cast<LPARAM>(icon_handle));
  }

  return error_code_t::OK;
}

static error_code_t windows_input_device_init()
{
  if (const lv_indev_t* pointer_indev = lv_windows_acquire_pointer_indev(g_display); 
    !pointer_indev)
  {
    return error_code_t::ERROR_POINTER_INDEV;
  }

  if (const lv_indev_t* keypad_indev = lv_windows_acquire_keypad_indev(g_display); 
    !keypad_indev)
  {
    return error_code_t::ERROR_KEYPAD_INDEV;
  }

  return error_code_t::OK;
}
#endif
