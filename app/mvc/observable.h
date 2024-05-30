/**
  ******************************************************************************
  * @file           : observable.h
  * @author         : Rusanov M.N.
  * @version        : V1.0.0
  * @date           : 30-May-2024
  * @brief          : Header for observable.cpp file.
  *                   This is the base class that manages the list of observers
  *                   and provides the ability to notify them of updates.
  *
  ******************************************************************************
  */

#pragma once

#include <vector>
#include "observer.h"

class observable
{
public:
  void add_observer(observer* observer);

protected:
  void notify_update() const;

private:
  std::vector<observer*> f_observers;
};