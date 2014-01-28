#pragma once

#include "lost/Event.h"
#include <mutex>

namespace lost
{

struct EventPool
{
  EventPool();
  ~EventPool();
  
  Event* borrowEvent();
  void returnEvent(Event* event);
  
  size_t          _numEvents;
  Event*          _events;
  std::mutex      _mutex;
};

}


