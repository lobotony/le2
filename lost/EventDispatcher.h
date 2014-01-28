#pragma once

#include "lost/Event.h"

namespace lost
{

// helper object: since function objects can't be compared, you get this back in order to reference the handler for later removal
struct EDConnection
{
  EventType eventType;
  u32 tag;
  
  EDConnection() : eventType(ET_None), tag(0) {}
  EDConnection(EventType et, u32 t) : eventType(et), tag(t) {}
};

typedef std::function<void(Event*)> EventHandler;

/** manages handlers (function objects/lambdas) that can be registered for specific event types
 *  * register a handler for a specific event type via addHandler. Keep track of the returned connection
 *    object if you need to remove the handler later on. Handlers are called in the order they were registered.
 *  * remove a handler via removeHandler, using the previously returned connection object
 *  * dispatch an event to all interested handlers by calling dispatchEvent()
 */
struct EventDispatcher
{

  struct TaggedHandler
  {
    EventHandler handler;
    u32 tag;
    TaggedHandler(const EventHandler& eh, u32 t) : handler(eh), tag(t) {}
  };
  typedef vector<TaggedHandler> Container;

  EventDispatcher();
  virtual ~EventDispatcher();
  
  EDConnection addHandler(EventType eventType, const EventHandler& handler);
  void removeHandler(const EDConnection& connection);
  void dispatchEvent(Event* event);
  
// private
  u32 currentTag;
  map<EventType, Container> type2container;
};
}


