#pragma once

/*********************
*      INCLUDES
*********************/
#include "misc/lv_color.h"

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void gui_create();
void print_text_message(const char* str);
void set_text_message_color(const lv_color_t color);
void set_btn_log_in_state(const bool is_active);
void set_btn_log_in_label(const char* str);
void set_ta_username_state(const bool is_active);
void set_ta_password_state(const bool is_active);
