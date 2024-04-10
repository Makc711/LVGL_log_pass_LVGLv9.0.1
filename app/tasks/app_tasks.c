/**
 * @file app_tasks.c
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
#endif

/**********************
*   STATIC VARIABLES
**********************/
#ifdef EMBEDDED
static osMutexId g_mutex_semaphore;
#endif
static char* g_entered_username;
static char* g_entered_password;

/**********************
*  STATIC PROTOTYPES
**********************/
#ifdef EMBEDDED
static void lvgl_handler(void* pvParameters);
static void task_mvc(void* pvParameters);
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void app_create_tasks(void)
{
#ifdef EMBEDDED
  g_mutex_semaphore = xSemaphoreCreateMutex();
  xTaskCreate(lvgl_handler, "lvgl_handler", 1000, NULL, osPriorityHigh, NULL);
  xTaskCreate(task_mvc, "task_mvc", 1000, NULL, osPriorityIdle, NULL);
#endif
}

void set_log_pass(const char* username, const char* password)
{
  if (g_entered_username)
  {
    free(g_entered_username);
  }
  if (g_entered_password)
  {
    free(g_entered_password);
  }

  g_entered_username = (char*)malloc(strlen(username) + 1);
  g_entered_password = (char*)malloc(strlen(password) + 1);

  if (g_entered_username && g_entered_password)
  {
    strcpy(g_entered_username, username);
    strcpy(g_entered_password, password);
  }
  else
  {
    // TODO: Memory allocation error handling
  }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
#ifdef EMBEDDED
static void lvgl_handler(void* pvParameters)
{
  for (;;)
  {
    xSemaphoreTake(g_mutex_semaphore, portMAX_DELAY);
    const uint32_t time_till_next = lv_task_handler();
    xSemaphoreGive(g_mutex_semaphore);
    osDelay(time_till_next);
  }
}

void vApplicationTickHook(void)
{
  lv_tick_inc(1);
}

void task_mvc(void* pvParameters)
{
  for (;;)
  {
    xSemaphoreTake(g_mutex_semaphore, portMAX_DELAY);
    run_mvc();
    xSemaphoreGive(g_mutex_semaphore);
    osDelay(30);
  }
}
#endif
void run_mvc(void)
{
  if (g_entered_username && g_entered_password)
  {
    mvc(g_entered_username, g_entered_password);
  }
}
