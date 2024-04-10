#pragma once

// Include
#include <cstring>
#include "app_tasks.h"

// Constants
constexpr int k_attempts = 3;

// Class auth_model
class auth_model
{
public:
  explicit auth_model(const char* username, const char* password)
    : f_username(username)
    , f_password(password)
    , f_counter(k_attempts) {}

  ~auth_model()
  {
    delete[] f_username;
    delete[] f_password;
  }

  bool authenticate(const char* username, const char* password)
  {
    app_mutex_lock();
    const bool is_equal = (strcmp(f_username, username) == 0) && (strcmp(f_password, password) == 0);
    app_mutex_unlock();

    if (!is_equal)
    {
      --f_counter;
    }
    return is_equal;
  }

  [[nodiscard]] int get_counter() const { return f_counter; }
  void reset_attempts() { f_counter = k_attempts; }

private:
  const char* f_username;
  const char* f_password;
  int f_counter;
};
