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
#include "memory.h"
#include "gui/gui_app.h"
#ifdef EMBEDDED
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
static error_code_t display_init(void);
static error_code_t input_device_init(void);
#ifndef EMBEDDED
static error_code_t windows_display_init(void);
static error_code_t windows_input_device_init(void);
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
error_code_t app_main(void)
{
#ifdef EMBEDDED
  hardware_init();
#endif

  lv_init();

  error_code_t res = display_init();
  if (res) { return res; }

  res = input_device_init();
  if (res) { return res; }

  app_create_tasks();
  gui_create();

  return OK;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static error_code_t display_init(void)
{
#ifdef EMBEDDED
  tft_init();
#else
  const error_code_t res = windows_display_init();
  if (res) { return res; }
#endif

  return OK;
}

static error_code_t input_device_init(void)
{
#ifdef EMBEDDED
  touchpad_init();
#else
  const error_code_t res = windows_input_device_init();
  if (res) { return res; }
#endif

  return OK;
}

#ifndef EMBEDDED
static error_code_t windows_display_init(void)
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
    return ERROR_CREATE_DISPLAY;
  }

  HWND window_handle = lv_windows_get_display_window_handle(g_display);
  if (!window_handle)
  {
    return ERROR_GET_DISPLAY_HANDLE;
  }

  HICON icon_handle = LoadIconW(
      GetModuleHandleW(NULL),
      MAKEINTRESOURCE(IDI_LVGL_WINDOWS));
  if (icon_handle)
  {
    SendMessageW(
        window_handle,
        WM_SETICON,
        TRUE,
        (LPARAM)icon_handle);
    SendMessageW(
        window_handle,
        WM_SETICON,
        FALSE,
        (LPARAM)icon_handle);
  }

  return OK;
}

static error_code_t windows_input_device_init(void)
{
  lv_indev_t* pointer_indev = lv_windows_acquire_pointer_indev(g_display);
  if (!pointer_indev)
  {
    return ERROR_POINTER_INDEV;
  }

  lv_indev_t* keypad_indev = lv_windows_acquire_keypad_indev(g_display);
  if (!keypad_indev)
  {
    return ERROR_KEYPAD_INDEV;
  }

  return OK;
}
#endif
