/**
 * @file app_tasks.cpp
 *
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
#ifdef EMBEDDED
#include "cmsis_os.h"
#else
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "processthreadsapi.h"
#include "WinBase.h"
#endif

/**********************
*      TYPEDEFS
**********************/
struct log_pass_t {
  const char* username;
  const char* password;
};

#ifdef EMBEDDED
using mutex_t = osMutexId;
#else
using mutex_t = std::mutex;
#endif

/**********************
*   STATIC VARIABLES
**********************/
static log_pass_t g_log_pass;
static mutex_t g_mutex_semaphore;
#ifdef EMBEDDED
static TaskHandle_t g_task_mvc_handle;
#else
static std::condition_variable g_cv;
#endif

/**********************
*  STATIC PROTOTYPES
**********************/
static void task_lvgl(void* pv_parameters);
static void task_mvc(void* pv_parameters);
static void sleep_os(uint32_t time_ms);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void app_create_tasks()
{
#ifdef EMBEDDED
  g_mutex_semaphore = xSemaphoreCreateMutex();
  TaskHandle_t g_task_lvgl_handle;
  xTaskCreate(task_lvgl, "task_lvgl", 900, nullptr, makeFreeRtosPriority(osPriorityNormal), &g_task_lvgl_handle);
  xTaskCreate(task_mvc, "task_mvc", 400, nullptr, uxTaskPriorityGet(g_task_lvgl_handle) + 1, &g_task_mvc_handle);
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

void update_log_pass(const char* username, const char* password)
{
  g_log_pass = { username, password };

#ifdef EMBEDDED
  xTaskNotifyGive(g_task_mvc_handle);
#else
  g_cv.notify_one();
#endif
}

void app_mutex_lock()
{
#ifdef EMBEDDED
  xSemaphoreTake(g_mutex_semaphore, portMAX_DELAY);
#else
  g_mutex_semaphore.lock();
#endif
}

void app_mutex_unlock()
{
#ifdef EMBEDDED
  xSemaphoreGive(g_mutex_semaphore);
#else
  g_mutex_semaphore.unlock();
#endif
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void task_lvgl(void* pv_parameters)
{
  gui_create();

  for (;;)
  {
    app_mutex_lock();
    const uint32_t time_till_next = lv_task_handler();
    app_mutex_unlock();
    sleep_os(time_till_next);
  }
}

void task_mvc(void* pv_parameters)
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
    if (ulTaskNotifyTake(pdFALSE, portMAX_DELAY) == 1)
    {
      if (model.is_auth_succeed())
      {
        controller.set_default_state();
      }
      else
      {
        controller.authenticate_user(g_log_pass.username, g_log_pass.password);
      }
    }
#else
    std::mutex m;
    std::unique_lock lock(m);
    g_cv.wait(lock);
    if (model.is_auth_succeed())
    {
      controller.set_default_state();
    }
    else
    {
      controller.authenticate_user(g_log_pass.username, g_log_pass.password);
    }
    lock.unlock();
#endif
  }
}

static void sleep_os(uint32_t time_ms)
{
#ifdef EMBEDDED
  osDelay(time_ms);
#else
  std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
#endif
}
