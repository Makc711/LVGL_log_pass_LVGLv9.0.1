/**
  ******************************************************************************
  * @file           : observer.h
  * @author         : Rusanov M.N.
  * @version        : V1.0.0
  * @date           : 30-May-2024
  * @brief          : This is an interface that defines a contract for classes
  *                   that wish to observe changes to observable.
  *                   
  ******************************************************************************
  */

#pragma once

class observer  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
  virtual ~observer() = default;
  virtual void update() = 0;
};