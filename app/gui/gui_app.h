/**
  ******************************************************************************
  * @file           : gui_app.h
  * @author         : Rusanov M.N.
  * @version        : V2.0.0
  * @date           : 30-May-2024
  * @brief          : Header for gui_app.cpp file.
  *                   This file contains functions for working GUI LVGL
  *
  ******************************************************************************
  */

#pragma once

#include "lvgl.h"

class gui_app
{
public:
  [[nodiscard]] static gui_app& get_instance();
  void set_text_message_color(lv_color_t color) const;
  void print_text_message(const char* str) const;
  [[nodiscard]] lv_obj_t* get_text_message() const;
  void set_ta_username_state(bool is_active) const;
  [[nodiscard]] const char* get_username() const;
  void set_ta_password_state(bool is_active) const;
  [[nodiscard]] const char* get_password() const;
  void set_btn_log_in_state(bool is_active) const;
  void set_btn_log_in_label(const char* str) const;

private:
  explicit gui_app();
  void init_style();
  void create_profile_panel(lv_obj_t* parent);
  static void ta_event_cb(lv_event_t* e);
  static void log_in_btn_event_cb(lv_event_t* e);

private:
  lv_style_t f_style_title{};
  lv_style_t f_style_text_muted{};
  lv_style_t f_style_text_normal{};

  const lv_font_t* f_font_large{};
  const lv_font_t* f_font_normal{};

  lv_obj_t* f_ta_username{};
  lv_obj_t* f_ta_password{};
  lv_obj_t* f_label_text_message{};
  lv_obj_t* f_btn_log_in{};
  lv_obj_t* f_label_btn_log_in{};
};
