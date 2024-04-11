#include "../../app/app.h"

int main()
{
  if (const error_code_t res = app_main();
    res != error_code_t::OK)
  {
    return static_cast<int>(res);
  }

  while (true) {}

  return 0;
}
