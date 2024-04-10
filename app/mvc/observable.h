#pragma once

// Include
#include <vector>
#include "observer.h"

// Class observable
class observable
{
public:
  void add_observer(observer* observer);

protected:
  void notify_update() const;

private:
  std::vector<observer*> f_observers;
};