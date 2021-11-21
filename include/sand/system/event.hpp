#pragma once

#include <vector>
namespace sand::system {
class EventPoller {
public:
  virtual void operator()() = 0;
  virtual bool should_quit() = 0;
};
} // namespace sand::system
