/**
  ******************************************************************************
  * @file           : app_tasks.cpp
  * @author         : Rusanov M.N.
  ******************************************************************************
  */

/*********************
 *      INCLUDES
 *********************/
#include "app_tasks.h"
#include "gui/lvgl/lvgl.h"
#include "gui/gui_app.h"
#include "memory.h"
#include "auth_model.h"
#include "auth_view.h"
#include "auth_controller.h"

#ifndef EMBEDDED
#include <iostream>
#include <thread>
#include <chrono>
#include "processthreadsapi.h"
#include "WinBase.h"
#endif

app_tasks::log_pass_t app_tasks::f_log_pass;
app_tasks::mutex_t app_tasks::f_mutex_semaphore;
#ifdef EMBEDDED
TaskHandle_t app_tasks::f_task_mvc_handle;
#else
std::condition_variable app_tasks::f_cv;
#endif

void app_tasks::init()
{
  if (static bool is_initialized = false; 
      !is_initialized)
  {
#ifdef EMBEDDED
    f_mutex_semaphore = xSemaphoreCreateMutex();
#endif
    create_tasks();
    is_initialized = true;
  }
}

void app_tasks::update_log_pass(const char* username, const char* password)
{
  f_log_pass = { username, password };

#ifdef EMBEDDED
  xTaskNotifyGive(f_task_mvc_handle);
#else
  f_cv.notify_one();
#endif
}

void app_tasks::mutex_lvgl_lock()
{
#ifdef EMBEDDED
  xSemaphoreTake(f_mutex_semaphore, portMAX_DELAY);
#else
  f_mutex_semaphore.lock();
#endif
}

void app_tasks::mutex_lvgl_unlock()
{
#ifdef EMBEDDED
  xSemaphoreGive(f_mutex_semaphore);
#else
  f_mutex_semaphore.unlock();
#endif
}

void app_tasks::create_tasks()
{
#ifdef EMBEDDED
  TaskHandle_t task_lvgl_handle;
  xTaskCreate(task_lvgl, "task_lvgl", 900, nullptr, makeFreeRtosPriority(osPriorityNormal), &task_lvgl_handle);
  xTaskCreate(task_mvc, "task_mvc", 400, nullptr, uxTaskPriorityGet(task_lvgl_handle) + 1, &f_task_mvc_handle);
#else
  std::thread lvgl_thread(task_lvgl, nullptr);
  std::thread mvc_thread(task_mvc, nullptr);

  if (!SetThreadPriority(lvgl_thread.native_handle(), THREAD_PRIORITY_NORMAL)) 
  {
    std::cerr << "Failed to set priority for LVGL thread\n";
  }

  if (!SetThreadPriority(mvc_thread.native_handle(), THREAD_PRIORITY_ABOVE_NORMAL)) 
  {
    std::cerr << "Failed to set priority for MVC thread\n";
  }

  lvgl_thread.detach();
  mvc_thread.detach();
#endif
}

void app_tasks::sleep_os(const uint32_t time_ms)
{
#ifdef EMBEDDED
  osDelay(time_ms);
#else
  std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
#endif
}

void app_tasks::task_lvgl(void* pv_parameters)
{
  gui_create();

  for (;;)
  {
    mutex_lvgl_lock();
    const uint32_t time_till_next = lv_task_handler();
    mutex_lvgl_unlock();
    sleep_os(time_till_next);
  }
}

void app_tasks::task_mvc(void* pv_parameters)
{
  char saved_username[k_username_max_length];
  char saved_password[k_password_max_length];
  memory_read_log_pass(saved_username, saved_password);

  auth_model model(saved_username, saved_password);
  const auth_view view(model);
  const auth_controller controller(model);

  for (;;)
  {
#ifdef EMBEDDED
    if (ulTaskNotifyTake(pdFALSE, portMAX_DELAY))
    {
      if (model.is_auth_succeed())
      {
        controller.set_default_state();
      }
      else
      {
        controller.authenticate_user(f_log_pass.username, f_log_pass.password);
      }
    }
#else
    std::mutex m;
    std::unique_lock lock(m);
    f_cv.wait(lock);
    if (model.is_auth_succeed())
    {
      controller.set_default_state();
    }
    else
    {
      controller.authenticate_user(f_log_pass.username, f_log_pass.password);
    }
    lock.unlock();
#endif
  }
}
