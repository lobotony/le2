#include "lost/EventSystem.h"
#include "lost/views/View.h"

namespace lost
{

EventSystem::EventSystem()
{
}
  
EventSystem::~EventSystem()
{
}

void EventSystem::propagateEvent(Event* event)
{
  DOUT("++++++++++++++++++++++++++++++++++++++");
  switch (event->base.type)
  {
    case ET_MouseUpEvent:
    case ET_MouseDownEvent:
    case ET_MouseMoveEvent:
      propagateMouseEvent(event);
      break;
    default:
      DOUT("don't know what to do with event of type: "<<event->base.type);
  }
  DOUT("--------------------------------------");
}

void EventSystem::propagateMouseEvent(Event* event)
{
  updateCurrentViewStack(event);
  if(currentViewStack.size() == 0) { return; } // bail if no views, happens when mouse outside of window

  logViewStack(currentViewStack);
  
  event->base.stopDispatch = false;
  event->base.stopPropagation = false;
  EventType et = event->base.type;
  
  if((et == ET_MouseUpEvent) || (et == ET_MouseDownEvent))
  {
    // FIXME: propagateUpDownEvents
  }
  else
  {
    // propagate scroll/move
    event->base.target = currentViewStack.back();
    s32 targetIndex = s32(currentViewStack.size())-1;
    propagateEvent(event, targetIndex);
  }
}

void EventSystem::propagateEvent(Event* event, s32 targetIndex)
{
  if(!event->base.stopPropagation) { propagateCaptureEvents(event, targetIndex); }
  if(!event->base.stopPropagation) { propagateTargetEvent(event, targetIndex); }
  if(!event->base.stopPropagation) { propagateBubbleEvents(event, targetIndex); }
}

void EventSystem::propagateCaptureEvents(Event* event, s32 targetIndex)
{
  // capture is only called on Views "before" the target,
  // i.e. in view hierarchies with only one view, no capture is called.
  for(u32 i=0; (i<targetIndex) && !event->base.stopPropagation && !event->base.stopDispatch; ++i)
  {
    View* view = currentViewStack[i];
    event->base.currentTarget = view;
    event->base.phase = EP_Capture;
    view->captureEventDispatcher.dispatchEvent(event);
  }
}

void EventSystem::propagateTargetEvent(Event* event, s32 targetIndex)
{
  // only called on lowermost view in the stack
  if(!event->base.stopDispatch && !event->base.stopPropagation)
  {
    View* view = currentViewStack[targetIndex];
    event->base.currentTarget = event->base.target;
    event->base.phase = EP_Target;
    view->targetEventDispatcher.dispatchEvent(event);
  }
}

void EventSystem::propagateBubbleEvents(Event* event, s32 targetIndex)
{
  // called on all views in the stack but the lowermost, in reverse order
  s32 i=targetIndex-1;
  while((i>=0) && !event->base.stopDispatch && !event->base.stopPropagation)
  {
    View* view = currentViewStack[i];
    event->base.currentTarget = view;
    event->base.phase = EP_Bubble;
    view->bubbleEventDispatcher.dispatchEvent(event);
    i--;
  }
}

void EventSystem::logViewStack(const vector<View*>& vs)
{
  for(auto v : vs)
  {
    DOUT("-> "<<v->name);
  }
}

void EventSystem::updateCurrentViewStack(Event* event)
{
  Vec2 pos(event->mouseEvent.x, event->mouseEvent.y);
  bool containsPoint = rootView->containsPoint(pos);
  View* view = rootView;
  currentViewStack.clear();
  
  while(containsPoint)
  {
    currentViewStack.push_back(view);
    containsPoint = false;

    for(auto i=view->subviews.rbegin(); i!=view->subviews.rend(); ++i)
    {
      View* v = (*i).get();
      if(v->isVisibleWithinSuperviews() && v->containsPoint(pos))
      {
        view = v;
        containsPoint = true;
        break;
      }
    }
  }
}

}

