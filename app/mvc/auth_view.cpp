// Include
#include "auth_view.h"
#include "src/stdlib/lv_sprintf.h"
#include "app_tasks.h"
#include "gui/gui_app.h"
#include "static_string.h"

using namespace snw1;

// Class auth_view
auth_view::auth_view(auth_model& model)
  : f_model(model)
{ 
  f_model.add_observer(this);
}

void auth_view::update()
{
  if (const auto attempts = f_model.get_attempts_counter(); 
    attempts != k_attempts)
  {
    if (f_model.is_auth_succeed())
    {
      show_success_message();
      set_profile_panel_mode(panel_mode_t::SUCCESS);
    }
    else
    {
      show_failure_message(attempts);
      if (attempts == 0)
      {
        set_profile_panel_mode(panel_mode_t::FAILURE);
      }
    }
  }
  else
  {
    set_profile_panel_mode(panel_mode_t::DEFAULT);
  }
}

void auth_view::set_profile_panel_mode(const panel_mode_t mode)
{
  app_mutex_lock();
  switch (mode)
  {
    case panel_mode_t::SUCCESS:
      set_ta_username_state(false);
      set_ta_password_state(false);
      set_btn_log_in_label("Log out");
      break;
    case panel_mode_t::FAILURE: 
      set_ta_username_state(false);
      set_ta_password_state(false);
      set_btn_log_in_state(false);
      break;
    case panel_mode_t::DEFAULT:
      set_ta_username_state(true);
      set_ta_password_state(true);
      set_btn_log_in_state(true);
      set_btn_log_in_label("Log in");
      print_text_message("");
      break;
  }
  app_mutex_unlock();
}

void auth_view::show_success_message()
{
  app_mutex_lock();
  set_text_message_color(lv_palette_main(LV_PALETTE_GREEN));
  print_text_message("Authentication is correct");
  app_mutex_unlock();
}

void auth_view::show_failure_message(const int attempts)
{
  app_mutex_lock();
  set_text_message_color(lv_palette_main(LV_PALETTE_RED));
  if (attempts)
  {
    constexpr auto failure_message = "Authentication is incorrect! Attempts left: ";
    constexpr auto message_size = c_strlen(failure_message) + ITOSS(k_attempts).length() + 1;
    char message[message_size];
    lv_snprintf(message, message_size, "%s%d", failure_message, attempts);
    print_text_message(message);
  }
  else
  {
    print_text_message("Entry blocked!");
  }
  app_mutex_unlock();
}
