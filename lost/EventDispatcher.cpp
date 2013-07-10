#include "lost/EventDispatcher.h"

namespace lost
{

EventDispatcher::EventDispatcher()
{
  currentTag = 0;
}

EventDispatcher::~EventDispatcher()
{
}

EDConnection EventDispatcher::addHandler(EventType eventType, const EventHandler& handler)
{
  u32 tag = currentTag;
  currentTag++;
  type2container[eventType].push_back(TaggedHandler(handler, tag));
  return EDConnection(eventType, tag);
}

void EventDispatcher::removeHandler(const EDConnection& connection)
{
  auto pos = type2container.find(connection.eventType);
  if(pos != type2container.end())
  {
    Container& container = pos->second;
    container.erase(std::remove_if(container.begin(), container.end(), [&](const TaggedHandler& th) {return (th.tag == connection.tag);}),
                    container.end());
  }
}

void EventDispatcher::dispatchEvent(Event* event)
{
  auto pos = type2container.find(event->base.type);
  if(pos != type2container.end())
  {
    for(auto th : pos->second)
    {
      th.handler(event);
    }
  }
}

}
