#pragma once

// Include
#include "observer.h"
#include "auth_model.h"

// Class auth_view
class auth_view final : public observer
{
public:
  explicit auth_view(auth_model& model);

private:
  enum class panel_mode_t
  {
    DEFAULT,
    SUCCESS,
    FAILURE
  };

  void update() override;
  static void show_success_message();
  static void show_failure_message(int attempts);
  static void set_profile_panel_mode(panel_mode_t mode);

private:
  auth_model& f_model;
};
