/**
  ******************************************************************************
  * @file           : auth_controller.h
  * @author         : Rusanov M.N.
  * @version        : V1.0.0
  * @date           : 30-May-2024
  * @brief          : Header for auth_controller.cpp file.
  *                   This file contains functions for working with MVC model
  *                   (Controller)
  *
  ******************************************************************************
  */

#pragma once

#include "auth_model.h"

class auth_controller
{
public:
  explicit auth_controller(auth_model& model);
  void authenticate_user(const char* username, const char* password) const;
  void set_default_state() const;

private:
  auth_model& f_model;
};
