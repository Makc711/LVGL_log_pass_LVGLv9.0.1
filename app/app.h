#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**********************
 *      TYPEDEFS
 **********************/
#ifdef __cplusplus
enum class error_code_t
#else
typedef enum
#endif
{
  OK                       =  0,
  ERROR_CREATE_DISPLAY     = -1,
  ERROR_GET_DISPLAY_HANDLE = -2,
  ERROR_POINTER_INDEV      = -3,
  ERROR_KEYPAD_INDEV       = -4
}
#ifdef __cplusplus
;
#else
error_code_t;
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
error_code_t app_main();

#ifdef __cplusplus
}
#endif
