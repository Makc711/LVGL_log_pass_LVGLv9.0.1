/**
  ******************************************************************************
  * @file           : auth_view.h
  * @author         : Rusanov M.N.
  * @version        : V2.0.0
  * @date           : 30-May-2024
  * @brief          : Header for auth_view.cpp file.
  *                   This file contains functions for working with MVC model
  *                   (View)
  *
  ******************************************************************************
  */

#pragma once

#include "observer.h"
#include "auth_model.h"
#include "gui/gui_app.h"

class auth_view final : public observer
{
public:
  static void init(auth_model& model);

private:
  enum class panel_mode_t
  {
    DEFAULT,
    SUCCESS,
    FAILURE
  };

  explicit auth_view(auth_model& model);
  void update() override;
  void show_success_message() const;
  void show_failure_message(int attempts) const;
  void set_profile_panel_mode(panel_mode_t mode) const;

private:
  auth_model& f_model;
  gui_app& f_gui_app;
};
