#include "lost/EventQueue.h"
#include "lost/EventPool.h"
#include "tinythread.h"

namespace lost
{

EventQueue::EventQueue()
{
  _mutex = new tthread::mutex;
  currentQ = &q0;
  nextQ = &q1;
}

EventQueue::~EventQueue()
{
  delete _mutex;
}

void EventQueue::addEventToNextQueue(Event* event)
{
  _mutex->lock();
  nextQ->push_back(event);
  _mutex->unlock();
}

const EventQueue::Container& EventQueue::getCurrentQueue()
{
  return *currentQ;
}

void EventQueue::swap()
{
  _mutex->lock();

  
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

  _mutex->unlock();
}

}

