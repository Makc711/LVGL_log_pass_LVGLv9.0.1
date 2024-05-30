/**
  ******************************************************************************
  * @file           : app_tasks.cpp
  * @author         : Rusanov M.N.
  ******************************************************************************
  */

#include "app_tasks.h"
#include "gui/lvgl/lvgl.h"
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
extern CRC_HandleTypeDef hcrc;
#else
std::condition_variable app_tasks::f_cv;
#endif

void app_tasks::init()
{
  for(static bool is_initialized = false; !is_initialized; is_initialized = true)
  {
#ifdef EMBEDDED
    f_mutex_semaphore = xSemaphoreCreateMutex();
#endif
    create_tasks();
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
#ifdef EMBEDDED
  const memory memory(flash_t::addr::ADDR_FLASH_SECTOR_7, hcrc);
#else
  const memory memory("log_pass.txt");
#endif
  data_t data{};
  [[maybe_unused]] const auto result = memory.read_data(data);

  auth_model model(data.username, data.password);
  auth_view::init(model);
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
