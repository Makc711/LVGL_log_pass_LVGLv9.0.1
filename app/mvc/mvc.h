#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void mvc_init(void); // Must be called after gui initialization!
void mvc(const char* username, const char* password);

#ifdef __cplusplus
}
#endif
