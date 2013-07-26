#include "lost/EventQueue.h"
#include "lost/EventPool.h"

namespace lost
{

EventQueue::EventQueue()
{
  currentQ = &q0;
  nextQ = &q1;
}

EventQueue::~EventQueue()
{
}

void EventQueue::addEventToNextQueue(Event* event)
{
  std::lock_guard<std::mutex> lock(mutex);

  nextQ->push_back(event);
}

const EventQueue::Container& EventQueue::getCurrentQueue()
{
  return *currentQ;
}

void EventQueue::swap()
{
  std::lock_guard<std::mutex> lock(mutex);

  
  // return all events to their respective pools and clear pointer list
  Container& cnt = *currentQ;

/*  size_t num = currentQ->size();
  if(num > 0)
  {
    DOUT("returning "<<(uint64_t)num<<" events");
  }*/

  for(Event* event : cnt)
  {
//    DOUT(event->base.type);
    event->base.pool->returnEvent(event);
  }
  cnt.clear();
  
  std::swap(currentQ, nextQ);
}

}

