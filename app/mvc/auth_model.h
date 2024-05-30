/**
  ******************************************************************************
  * @file           : auth_model.h
  * @author         : Rusanov M.N.
  * @version        : V1.0.0
  * @date           : 30-May-2024
  * @brief          : Header for auth_model.cpp file.
  *                   This file contains functions for working with MVC model
  *                   (Model)
  *
  ******************************************************************************
  */

#pragma once

#include "observable.h"

constexpr auto k_attempts = 3;

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
