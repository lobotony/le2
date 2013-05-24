#ifndef LOST_EVENTQUEUE_H
#define LOST_EVENTQUEUE_H

#include "lost/Event.h"

namespace tthread { class mutex; }

namespace lost
{

/** double buffered, thread safe event queue.
 * Borrow events from a EventPool.
 * Store them here for later processing.
 *
 * Event consuming code and event generating code will always run in seperate threads.
 * Events will returned to the pool in bulk once the current frame has been completely processed.
 * During the processing of the current queue, the "next" queue will be filled by the other thread
 * with new incoming events. 
 * 
 * This prevents events from being dropped. Using only a single queue can lead to scenarios where events
 * are added to a queue AFTER the engine processing and BEFORE the current frame ends and events are cleaned
 * up, leading to events simply being dropped and changes not being communicated to the engine.
 *
 */
struct EventQueue
{
  typedef vector<Event*> Container;

  EventQueue();
  ~EventQueue();

  void addEventToNextQueue(Event* event); // add an event to the queue for the next frame. Use this from the UI thread
  Container& getCurrentQueue(); // returns the queue whose events should be read and consumed next. Use this from the engine thread
  void swap();
  

private:
  tthread::mutex* _mutex;
  Container* currentQ;
  Container* nextQ;
  Container q0;
  Container q1;
};

}
#endif

