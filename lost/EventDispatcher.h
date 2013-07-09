#ifndef LOST_EVENTDISPATCHER_H
#define LOST_EVENTDISPATCHER_H

#include <functional>
#include "lost/Event.h"

namespace lost
{
struct EventDispatcher
{
  EventDispatcher();
  virtual ~EventDispatcher();
  
private:
  map<EventType, vector<std::function<void(Event*)>>> type2handlers;
};
}

#endif

