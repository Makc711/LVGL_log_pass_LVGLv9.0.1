#pragma once

// Include
#include <string>
#include "gui/gui_app.h"

// Class AuthView
class auth_view
{
public:
  void show_success_message() const
  {
    print_text_message("Authentication is correct");
  }

  void show_failure_message(const int attempts) const
  {
    if (attempts)
    {
      const std::string message = "Authentication is incorrect! Attempts left: " + std::to_string(attempts);
      print_text_message(message.c_str());
    }
    else
    {
      print_text_message("Entry blocked!");
      set_log_in_btn_inactive();
    }
  }
};
