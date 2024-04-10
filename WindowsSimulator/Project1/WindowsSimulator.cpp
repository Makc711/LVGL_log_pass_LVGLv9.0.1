#include "../../app/app.h"

int main()
{
  if (const error_code_t res = app_main())
  {
    return res;
  }

  while (true) {}

  return 0;
}
