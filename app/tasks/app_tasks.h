/**
  ******************************************************************************
  * @file           : app_tasks.h
  * @author         : Rusanov M.N.
  * @version        : V2.0.0
  * @date           : 29-May-2024
  * @brief          : Header for app_tasks.cpp file.
  *                   This file contains functions for creating and working with
  *                   freeRTOS tasks.
  *
  ******************************************************************************
  */

#pragma once

#ifdef EMBEDDED
#include "cmsis_os.h"
#else
#include <mutex>
#endif

class app_tasks
{
public:
  static void init();
  static void update_log_pass(const char* username, const char* password);
  static void mutex_lvgl_lock();
  static void mutex_lvgl_unlock();

private:
  static void create_tasks();
  static void sleep_os(uint32_t time_ms);
  static void task_lvgl(void* pv_parameters);
  static void task_mvc(void* pv_parameters);

private:
  struct log_pass_t
  {
    const char* username;
    const char* password;
  };

#ifdef EMBEDDED
  using mutex_t = osMutexId;
#else
  using mutex_t = std::mutex;
#endif

  static log_pass_t f_log_pass;
  static mutex_t f_mutex_semaphore;
#ifdef EMBEDDED
  static TaskHandle_t f_task_mvc_handle;
#else
  static std::condition_variable f_cv;
#endif
};