// Include
#include "auth_controller.h"

// Class auth_controller
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
