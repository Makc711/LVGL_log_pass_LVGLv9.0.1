#include <cstdint>
#include "../../app/app.h"
#include "../../app/gui/lvgl/lvgl.h"
#include "../../app/tasks/app_tasks.h"

int main()
{
  if (const error_code_t res = app_main())
  {
    return res;
  }

  while (true)
  {
    const uint32_t time_till_next = lv_task_handler();
    Sleep(time_till_next);
    run_mvc();
  }

  return 0;
}
