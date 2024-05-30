/**
  ******************************************************************************
  * @file           : observable.cpp
  * @author         : Rusanov M.N.
  ******************************************************************************
  */

#include "observable.h"

void observable::add_observer(observer* observer)
{
  f_observers.push_back(observer);
}

void observable::notify_update() const
{
  for (const auto& observer : f_observers) 
  {
    observer->update();
  }
}
