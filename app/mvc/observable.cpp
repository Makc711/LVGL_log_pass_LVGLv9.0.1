// Include
#include "observable.h"

// Class observable
void observable::add_observer(observer* observer)
{
  f_observers.push_back(observer);
}

void observable::notify_update() const
{
  const size_t size = f_observers.size();
  for (size_t i = 0; i < size; i++)
  {
    f_observers[i]->update();
  }
}
