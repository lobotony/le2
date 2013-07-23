#ifndef LOST_EVENT_H
#define LOST_EVENT_H

namespace lost
{

enum EventType
{
  ET_None=0,
  ET_KeyDown,
  ET_KeyUp,
  ET_MouseDown,
  ET_MouseUp,
  ET_MouseMove,
  ET_MouseUpOutside,
  ET_MouseUpInside,
  ET_MouseEnter,
  ET_MouseLeave,
  ET_FocusLost,
  ET_FocusGained,
  ET_WindowResize
};

// for ui
enum EventPhase
{
  EP_None=0,
  EP_Capture,
  EP_Target,
  EP_Bubble
};

struct EventPool;

struct BaseEvent
{
  EventType   type;
  bool        used;
  EventPool*  pool;
  
  // ui
  bool        bubbles;
  bool        stopDispatch;
  bool        stopPropagation;
  EventPhase  phase;
  View*       target;
  View*       currentTarget;
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

static const char* eventTypeToString(EventType et)
{
  const char* result = "<event?>";
  
  switch(et)
  {
    case ET_None:result="ET_None";break;
    case ET_KeyDown:result="ET_KeyDown";break;
    case ET_KeyUp:result="ET_KeyUp";break;
    case ET_MouseDown:result="ET_MouseDown";break;
    case ET_MouseUp:result="ET_MouseUp";break;
    case ET_MouseMove:result="ET_MouseMove";break;
    case ET_MouseUpOutside:result="ET_MouseUpOutside";break;
    case ET_MouseUpInside:result="ET_MouseUpInside";break;
    case ET_MouseEnter:result="ET_MouseEnter";break;
    case ET_MouseLeave:result="ET_MouseLeave";break;
    case ET_FocusLost:result="ET_FocusLost";break;
    case ET_FocusGained:result="ET_FocusGained";break;
    case ET_WindowResize:result="ET_WindowResize";break;
  }
  
  return result;
}

static const char* eventPhaseToString(EventPhase ep)
{
  const char* result = "<phase?>";
  
  switch(ep)
  {
    case EP_None:result="EP_None";break;
    case EP_Capture:result="EP_Capture";break;
    case EP_Target:result="EP_Target";break;
    case EP_Bubble:result="EP_Bubble";break;
  }
  
  return result;
}

}

#endif

