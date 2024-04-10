/**
 * @file app_tasks.cpp
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "app_tasks.h"
#include "gui/lvgl/lvgl.h"
#include "mvc.h"
#ifdef EMBEDDED
#include "cmsis_os.h"
#else
#include <thread>
#include <chrono>
#include <mutex>
#endif

/**********************
*      TYPEDEFS
**********************/
#ifdef EMBEDDED
using mutex_t = osMutexId;
#else
using mutex_t = std::mutex;
#endif

/**********************
*   STATIC VARIABLES
**********************/
static mutex_t g_mutex_semaphore;
static char* g_entered_username = new char[1]();
static char* g_entered_password = new char[1]();
static bool g_is_log_pass_not_checked = false;

/**********************
*  STATIC PROTOTYPES
**********************/
static void lvgl_handler(void* pv_parameters);
static void task_mvc(void* pv_parameters);
static void sleep_os(uint32_t time_ms);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void app_create_tasks(void)
{
#ifdef EMBEDDED
  g_mutex_semaphore = xSemaphoreCreateMutex();
  xTaskCreate(lvgl_handler, "lvgl_handler", 1000, NULL, osPriorityHigh, NULL);
  xTaskCreate(task_mvc, "task_mvc", 1000, NULL, osPriorityIdle, NULL);
#else
  std::thread th1(lvgl_handler, nullptr);
  th1.detach();
  std::thread th2(task_mvc, nullptr);
  th2.detach();
#endif
}

void update_log_pass(const char* username, const char* password)
{
  if ((strcmp(g_entered_username, username) != 0) || (strcmp(g_entered_password, password) != 0))
  {
    delete[] g_entered_username;
    delete[] g_entered_password;

    g_entered_username = new char[strlen(username) + 1];
    g_entered_password = new char[strlen(password) + 1];

    if (g_entered_username && g_entered_password)
    {
      strcpy(g_entered_username, username);
      strcpy(g_entered_password, password);
      g_is_log_pass_not_checked = true;
    }
    else
    {
      while (true) {} // TODO: Memory allocation error handling
    }
  }
}

void app_mutex_lock(void)
{
#ifdef EMBEDDED
  xSemaphoreTake(g_mutex_semaphore, portMAX_DELAY);
#else
  g_mutex_semaphore.lock();
#endif
}

void app_mutex_unlock(void)
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
static void lvgl_handler(void* pv_parameters)
{
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
  for (;;)
  {
    if (g_is_log_pass_not_checked)
    {
      mvc(g_entered_username, g_entered_password);
      g_is_log_pass_not_checked = false;
    }
    sleep_os(30);
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
