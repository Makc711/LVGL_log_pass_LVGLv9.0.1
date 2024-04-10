#pragma once

/*********************
 *     CONSTANTS
 *********************/
constexpr auto k_default_username = "Maxim";
constexpr auto k_default_password = "123456";
constexpr auto k_username_max_length = 20;
constexpr auto k_password_max_length = 20;
constexpr auto k_password_min_length = 6;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void memory_read_log_pass(char* username, char* password);
