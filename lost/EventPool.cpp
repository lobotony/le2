#include "lost/EventPool.h"

namespace lost
{

EventPool::EventPool()
{
  _numEvents = 128;
  size_t bytes = _numEvents*sizeof(Event);
  _events = (Event*)malloc(bytes);
  memset(_events, 0, bytes);
  for(size_t i=0; i<_numEvents; ++i)
  {
    Event* event = &_events[i];
    event->base.pool = this;
  }
}

EventPool::~EventPool()
{
  free(_events);
}

Event* EventPool::borrowEvent()
{
  std::lock_guard<std::mutex> lock(_mutex);
  Event* result = NULL;
  
  for(size_t i=0; i<_numEvents; ++i)
  {
    if(_events[i].base.used == false)
    {
      result = &_events[i];
      result->base.used = true;
      break;
    }
  }
  ASSERT(result, "no free event found");
  return result;
}

void EventPool::returnEvent(Event* event)
{
  std::lock_guard<std::mutex> lock(_mutex);
  event->base.used = false;
}

}

