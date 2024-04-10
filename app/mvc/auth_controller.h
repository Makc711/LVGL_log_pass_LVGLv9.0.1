#pragma once

// Include
#include "auth_model.h"
#include "auth_view.h"

// Class auth_controller
class auth_controller
{
public:
  explicit auth_controller(auth_model* model)
      : f_model(model) {}

  ~auth_controller()
  {
    delete f_model;
    f_model = nullptr;
  }

  void authenticate_user(const char* username, const char* password) const
  {
    if (f_model->authenticate(username, password))
    {
      f_model->reset_attempts();
      f_view.show_success_message();
    }
    else
    {
      const int attempts = f_model->get_counter();
      f_view.show_failure_message(attempts);
    }
  }

private:
  auth_model* f_model;
  auth_view f_view;
};
