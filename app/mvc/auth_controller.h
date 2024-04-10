#pragma once

// Include
#include "auth_model.h"

// Class auth_controller
class auth_controller
{
public:
  explicit auth_controller(auth_model& model);
  void authenticate_user(const char* username, const char* password) const;
  void set_default_state() const;

private:
  auth_model& f_model;
};
