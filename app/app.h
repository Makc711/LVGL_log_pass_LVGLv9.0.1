#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef enum
{
  OK                       = 0,
  ERROR_CREATE_DISPLAY     = -1,
  ERROR_GET_DISPLAY_HANDLE = -2,
  ERROR_POINTER_INDEV      = -3,
  ERROR_KEYPAD_INDEV       = -4
} error_code_t;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
error_code_t app_main(void);

#ifdef __cplusplus
}
#endif
