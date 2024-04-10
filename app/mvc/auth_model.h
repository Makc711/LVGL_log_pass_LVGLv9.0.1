#pragma once

// Include
#include <string>

// Constants
constexpr int k_attempts = 3;

// Class auth_model
class auth_model
{
public:
    explicit auth_model(std::string username, std::string password)
        : f_username(std::move(username))
        , f_password(std::move(password))
        , f_counter(k_attempts) {}

    bool authenticate(const std::string& username, const std::string& password)
    {
        bool result = false;
        if ((username == f_username) && (password == f_password))
        {
            result = true;
        }
        else
        {
            --f_counter;
        }
        return result;
    }

    [[nodiscard]] int get_counter() const { return f_counter; }
    void reset_attempts() { f_counter = k_attempts; }

private:
    std::string f_username;
    std::string f_password;
    int f_counter;
};
