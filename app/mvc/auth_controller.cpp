/**
  ******************************************************************************
  * @file           : auth_controller.cpp
  * @author         : Rusanov M.N.
  ******************************************************************************
  */

#include "auth_controller.h"

auth_controller::auth_controller(auth_model& model)
  : f_model(model) {}

void auth_controller::authenticate_user(const char* username, const char* password) const
{
  f_model.authenticate(username, password);
}

void auth_controller::set_default_state() const
{
  f_model.reset();
}
