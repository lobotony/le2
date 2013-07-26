#ifndef LOST_EVENTPOOL_H
#define LOST_EVENTPOOL_H

#include "lost/Event.h"
#include <thread>

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

#endif

