#ifndef LOST_EVENT_H
#define LOST_EVENT_H

namespace lost
{

enum EventType
{
  ET_None=0,
  ET_KeyDownEvent,
  ET_KeyUpEvent,
  ET_MouseDownEvent,
  ET_MouseUpEvent,
  ET_MouseMoveEvent,
  ET_WindowResize
};

struct EventPool;

struct BaseEvent
{
  EventType   type;
  bool        used;
  EventPool*  pool;
};

struct KeyEvent : BaseEvent
{
  int32_t keyCode;
};

struct MouseEvent : BaseEvent
{
  s32 x;
  s32 y;
};

// don't add default constructors to event classes, or the union won't compile
union Event
{
  BaseEvent         base;
  KeyEvent          keyEvent;
  MouseEvent        mouseEvent;
};

}

#endif

