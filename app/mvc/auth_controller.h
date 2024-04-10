#pragma once

// Include
#include "auth_model.h"
#include "auth_view.h"

// Class auth_controller
class auth_controller
{
public:
    explicit auth_controller(auth_model* model, auth_view* view)
        : f_model(model)
        , f_view(view) {}

    void authenticate_user(const std::string& username, const std::string& password) const
    {
        if (f_model->authenticate(username, password))
        {
            f_model->reset_attempts();
            f_view->show_success_message();
        }
        else
        {
            const int attempts = f_model->get_counter();
            f_view->show_failure_message(attempts);
        }
    }

private:
    auth_model* f_model;
    auth_view* f_view;
};
