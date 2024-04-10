#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      DEFINES
 *********************/
typedef enum
{
  ok                       = 0,
  error_create_display     = -1,
  error_get_display_handle = -2,
  error_pointer_indev      = -3,
  error_keypad_indev       = -4
} error_code_t;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
error_code_t app_main(void);

#ifdef __cplusplus
}
#endif
