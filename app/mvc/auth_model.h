#pragma once

// Include
#include <memory>
#include "observable.h"

// Constants
constexpr auto k_attempts = 3;

// Class auth_model
class auth_model final : public observable
{
public:
  explicit auth_model(const char* username, const char* password);
  [[nodiscard]] int get_attempts_counter() const;
  [[nodiscard]] bool is_auth_succeed() const;

private:
  friend class auth_controller;
  void authenticate(const char* username, const char* password);
  void reset();

private:
  const char* f_username;
  const char* f_password;
  int f_counter;
  bool f_is_auth_succeed;
};
