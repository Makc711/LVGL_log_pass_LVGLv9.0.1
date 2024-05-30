/**
  ******************************************************************************
  * @file           : auth_model.cpp
  * @author         : Rusanov M.N.
  ******************************************************************************
  */

#include "auth_model.h"
#include <cstring>

auth_model::auth_model(const char* username, const char* password)
  : f_username(username)
  , f_password(password)
  , f_counter(k_attempts)
  , f_is_auth_succeed(false) {}

int auth_model::get_attempts_counter() const { return f_counter; }
bool auth_model::is_auth_succeed() const { return f_is_auth_succeed; }

void auth_model::authenticate(const char* username, const char* password)
{
  f_is_auth_succeed = (strcmp(f_username, username) == 0) && (strcmp(f_password, password) == 0);
  --f_counter;
  notify_update();
}

void auth_model::reset()
{
  f_is_auth_succeed = false;
  f_counter = k_attempts;
  notify_update();
}
