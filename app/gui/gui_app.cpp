/**
  ******************************************************************************
  * @file           : gui_app.cpp
  * @author         : Rusanov M.N.
  ******************************************************************************
  */

#include "gui_app.h"
#include "app_tasks.h"
#include "memory.h"
#include "static_string.h"

using namespace snw1;

gui_app::gui_app()
{
  init_style();
  create_profile_panel(lv_screen_active());
}

gui_app& gui_app::get_instance()
{
  static gui_app instance;
  return instance;
}

void gui_app::set_text_message_color(const lv_color_t color) const
{
  lv_obj_set_style_text_color(f_label_text_message, color, LV_PART_MAIN);
}

void gui_app::print_text_message(const char* str) const
{
  lv_label_set_text(f_label_text_message, str);
}

lv_obj_t* gui_app::get_text_message() const
{
  return f_label_text_message;
}

void gui_app::set_ta_username_state(const bool is_active) const
{
  lv_obj_set_state(f_ta_username, LV_STATE_DISABLED, !is_active);
}

const char* gui_app::get_username() const
{
  return lv_textarea_get_text(f_ta_username);
}

void gui_app::set_ta_password_state(const bool is_active) const
{
  lv_obj_set_state(f_ta_password, LV_STATE_DISABLED, !is_active);
}

const char* gui_app::get_password() const
{
  return lv_textarea_get_text(f_ta_password);
}

void gui_app::set_btn_log_in_state(const bool is_active) const
{
  lv_obj_set_state(f_btn_log_in, LV_STATE_DISABLED, !is_active);
}

void gui_app::set_btn_log_in_label(const char* str) const
{
  lv_label_set_text(f_label_btn_log_in, str);
}

void gui_app::init_style()
{
  f_font_normal = LV_FONT_DEFAULT;
  f_font_large = LV_FONT_DEFAULT;

#if LV_FONT_MONTSERRAT_14
  f_font_normal = &lv_font_montserrat_14;
#else
  LV_LOG_WARN("LV_FONT_MONTSERRAT_14 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif

#if LV_FONT_MONTSERRAT_20
  f_font_large = &lv_font_montserrat_20;
#else
  LV_LOG_WARN("LV_FONT_MONTSERRAT_20 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif

#if LV_USE_THEME_DEFAULT
  lv_theme_default_init(nullptr,
    lv_palette_main(LV_PALETTE_BLUE),
    lv_palette_main(LV_PALETTE_RED),
    LV_THEME_DEFAULT_DARK,
    f_font_normal);
#endif

  lv_style_init(&f_style_title);
  lv_style_set_text_font(&f_style_title, f_font_large);

  lv_style_init(&f_style_text_muted);
  lv_style_set_text_font(&f_style_title, f_font_normal);
  lv_style_set_text_opa(&f_style_text_muted, LV_OPA_50);

  lv_style_init(&f_style_text_normal);
  lv_style_set_text_color(&f_style_text_normal, lv_color_black());
  lv_style_set_text_font(&f_style_text_normal, f_font_normal);
}

void gui_app::create_profile_panel(lv_obj_t* parent)
{
  lv_obj_t* panel_profile = lv_obj_create(parent);
  
  lv_obj_t* kb = lv_keyboard_create(parent);
  lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

  lv_obj_t* panel_profile_title = lv_label_create(panel_profile);
  lv_label_set_text(panel_profile_title, "Log in to your profile");
  lv_obj_add_style(panel_profile_title, &f_style_title, LV_PART_MAIN);

  lv_obj_t* label_username = lv_label_create(panel_profile);
  lv_label_set_text(label_username, "User name");
  lv_obj_add_style(label_username, &f_style_text_muted, LV_PART_MAIN);

  f_ta_username = lv_textarea_create(panel_profile);
  lv_textarea_set_one_line(f_ta_username, true);
  lv_textarea_set_placeholder_text(f_ta_username, "Your name");
  lv_obj_add_event_cb(f_ta_username, ta_event_cb, LV_EVENT_ALL, kb);

  lv_obj_t* label_password = lv_label_create(panel_profile);
  lv_label_set_text(label_password, "Password");
  lv_obj_add_style(label_password, &f_style_text_muted, LV_PART_MAIN);

  f_ta_password = lv_textarea_create(panel_profile);
  lv_textarea_set_one_line(f_ta_password, true);
  lv_textarea_set_password_mode(f_ta_password, true);
  constexpr auto bs_str = "Min. " + ITOSS(k_password_min_length) + " chars.";
  lv_textarea_set_placeholder_text(f_ta_password, bs_str.c_str());
  lv_obj_add_event_cb(f_ta_password, ta_event_cb, LV_EVENT_ALL, kb);

  f_label_text_message = lv_label_create(panel_profile);
  lv_label_set_text(f_label_text_message, "");
  lv_obj_add_style(f_label_text_message, &f_style_text_normal, LV_PART_MAIN);

  f_btn_log_in = lv_button_create(panel_profile);
  lv_obj_set_height(f_btn_log_in, LV_SIZE_CONTENT);
  lv_obj_set_width(f_btn_log_in, 120);
  lv_obj_add_event_cb(f_btn_log_in, log_in_btn_event_cb, LV_EVENT_CLICKED, nullptr);

  f_label_btn_log_in = lv_label_create(f_btn_log_in);
  lv_label_set_text(f_label_btn_log_in, "Log in");
  lv_obj_center(f_label_btn_log_in);

  static int32_t grid_main_col_dsc[] = { LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
  static int32_t grid_main_row_dsc[] = { LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };

  static int32_t grid_1_col_dsc[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
  static int32_t grid_1_row_dsc[] = {
    LV_GRID_CONTENT, /*Title*/
    5, /*Separator*/
    LV_GRID_CONTENT, /*Box title*/
    40, /*Box*/
    5, /*Separator*/
    LV_GRID_CONTENT, /*Button*/
    5, /*Separator*/
    LV_GRID_CONTENT, /*Text message*/
    LV_GRID_TEMPLATE_LAST
  };

  lv_obj_set_grid_dsc_array(parent, grid_main_col_dsc, grid_main_row_dsc);
  lv_obj_set_grid_cell(panel_profile, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

  lv_obj_set_grid_dsc_array(panel_profile, grid_1_col_dsc, grid_1_row_dsc);
  lv_obj_set_grid_cell(panel_profile_title, LV_GRID_ALIGN_CENTER, 1, 2, LV_GRID_ALIGN_CENTER, 0, 1);
  lv_obj_set_grid_cell(label_username, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_START, 2, 1);
  lv_obj_set_grid_cell(label_password, LV_GRID_ALIGN_CENTER, 2, 2, LV_GRID_ALIGN_START, 2, 1);
  lv_obj_set_grid_cell(f_ta_username, LV_GRID_ALIGN_STRETCH, 0, 2, LV_GRID_ALIGN_START, 3, 1);
  lv_obj_set_grid_cell(f_ta_password, LV_GRID_ALIGN_STRETCH, 2, 2, LV_GRID_ALIGN_START, 3, 1);
  lv_obj_set_grid_cell(f_btn_log_in, LV_GRID_ALIGN_CENTER, 1, 2, LV_GRID_ALIGN_CENTER, 5, 1);
  lv_obj_set_grid_cell(f_label_text_message, LV_GRID_ALIGN_CENTER, 0, 4, LV_GRID_ALIGN_CENTER, 7, 1);
}

void gui_app::ta_event_cb(lv_event_t* e)
{
  lv_obj_t* parent = lv_screen_active();
  const auto ta = static_cast<lv_obj_t*>(lv_event_get_target(e));
  const auto kb = static_cast<lv_obj_t*>(lv_event_get_user_data(e));

  if (const lv_event_code_t code = lv_event_get_code(e); 
    code == LV_EVENT_CLICKED)
  {
    if (lv_indev_get_type(lv_indev_active()) != LV_INDEV_TYPE_KEYPAD)
    {
      lv_keyboard_set_textarea(kb, ta);
      lv_obj_set_style_max_height(kb, LV_HOR_RES * 2 / 3, LV_PART_MAIN);
      lv_obj_update_layout(parent);   /*Be sure the sizes are recalculated*/
      lv_obj_set_height(parent, LV_VER_RES - lv_obj_get_height(kb));
      lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
      lv_obj_scroll_to_view_recursive(ta, LV_ANIM_ON);
      lv_indev_wait_release(static_cast<lv_indev_t*>(lv_event_get_param(e)));
    }
  }
  else if (code == LV_EVENT_DEFOCUSED)
  {
    lv_keyboard_set_textarea(kb, nullptr);
    lv_obj_set_height(parent, LV_VER_RES);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_indev_reset(nullptr, ta);
  }
  else if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)
  {
    lv_obj_set_height(parent, LV_VER_RES);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_indev_reset(nullptr, ta);   /*To forget the last clicked object to make it focusable again*/
  }
}

void gui_app::log_in_btn_event_cb(lv_event_t* e)
{
  const auto gui_app = get_instance();
  const auto text_message = gui_app.get_text_message();
  const auto username = gui_app.get_username();

  if (const auto password = gui_app.get_password(); 
    (username[0] == '\0') || (password[0] == '\0'))
  {
    lv_obj_set_style_text_color(text_message, lv_color_black(), LV_PART_MAIN);
    lv_label_set_text(text_message, "The username or password cannot be empty");
  }
  else
  {
    if (strlen(password) < k_password_min_length)
    {
      lv_obj_set_style_text_color(text_message, lv_color_black(), LV_PART_MAIN);
      constexpr auto bs_str = "Password must be greater than " + ITOSS(k_password_min_length) + " chars.";
      lv_label_set_text(text_message, bs_str.c_str());
    }
    else
    {
      app_tasks::update_log_pass(username, password);
    }
  }
}
