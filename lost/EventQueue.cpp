#include "lost/EventQueue.h"
#include "lost/EventPool.h"
#include "tinythread.h"

namespace lost
{

EventQueue::EventQueue()
{
  _mutex = new tthread::mutex;
}

EventQueue::~EventQueue()
{
  delete _mutex;
}

void EventQueue::addEvent(Event* event)
{
  _mutex->lock();
  events.push_back(event);
  _mutex->unlock();
}

void EventQueue::returnAllEvents()
{
  size_t num = events.size();
  if(num > 0)
  {
//    DOUT("returning "<<(uint64_t)num<<" events");
  }
  for(size_t i=0; i<num; ++i)
  {
    Event* event = events[i];
    event->base.pool->returnEvent(event);
  }
  events.clear();
}

}

