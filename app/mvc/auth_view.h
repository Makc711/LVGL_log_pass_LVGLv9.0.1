#pragma once

// Include
#include <string>
#include "gui/gui_app.h"
#include "lvgl.h"

// Class AuthView
class auth_view
{
public:
    explicit auth_view(lv_obj_t* text_message_label)
        : f_text_message_label(text_message_label) {}

    void show_success_message() const
    {
        lv_label_set_text(f_text_message_label, "Authentication is correct");
    }

    void show_failure_message(const int attempts) const
    {
        if (attempts)
        {
            const std::string message = "Authentication is incorrect! Attempts left: " + std::to_string(attempts);
            lv_label_set_text(f_text_message_label, message.c_str());
        }
        else
        {
            lv_label_set_text(f_text_message_label, "Entry blocked!");
            set_log_in_btn_inactive();
        }
    }

private:
    lv_obj_t* f_text_message_label;
};
