#pragma once

class observer
{
public:
  virtual ~observer() = default;
  virtual void update() = 0;
};