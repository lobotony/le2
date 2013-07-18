#include "lost/EventSystem.h"
#include "lost/views/View.h"
#include "lost/Application.h"
#include "lost/EventPool.h"

namespace lost
{

EventSystem::EventSystem()
{
  rootView = NULL;
  reset();
}
  
EventSystem::~EventSystem()
{
}

void EventSystem::viewDying(View* view)
{
  // FIXME: remove from all members and re-establish consistent state afterwards
  DOUT(view->name());
  
#define rm(vec, elem) {auto pos = find(vec.begin(), vec.end(), elem); if(pos != vec.end()) { DOUT(#vec<<" removing "<<elem->name());vec.erase(pos);}}

  loseFocus(view); // make sure current focus and its stack are consistent

  rm(currentViewStack, view);
  rm(previousMouseMoveStack, view);
  rm(previousMouseClickStack, view);
  rm(currentFocusStack, view);
  rm(previousFocusStack, view);
}

void EventSystem::reset()
{
  currentViewStack.clear();
  previousMouseMoveStack.clear();
  previousMouseClickStack.clear();
  previousFocusStack.clear();
  currentFocusStack.clear();
  focusChanged = false;
  currentlyFocusedView = rootView;
}

void EventSystem::propagateEvent(Event* event)
{
//  DOUT("++++++++++++++++++++++++++++++++++++++");
  switch (event->base.type)
  {
    case ET_MouseUp:
    case ET_MouseDown:
    case ET_MouseMove:
      propagateMouseEvent(event);
      break;
    case ET_WindowResize:
      break;
    default:
      DOUT("don't know what to do with event of type: "<<event->base.type);
  }
//  DOUT("--------------------------------------");
}

void EventSystem::propagateMouseEvent(Event* event)
{
  updateCurrentViewStack(event);
  if(currentViewStack.size() == 0) { return; } // bail if no views, happens when mouse outside of window

//  logViewStack(currentViewStack);
  
  event->base.bubbles = true;
  event->base.stopDispatch = false;
  event->base.stopPropagation = false;
  EventType et = event->base.type;
  
  if((et == ET_MouseUp) || (et == ET_MouseDown))
  {
    propagateUpDownEvent(event);
    if(et == ET_MouseDown)
    {
      propagateFocusEvent(event);
    }
  }
  else
  {
    // propagate scroll/move
    event->base.target = currentViewStack.back();
    s32 targetIndex = s32(currentViewStack.size())-1;
    propagateEvent(currentViewStack, event, targetIndex);
  }
  
  // enter/leave
  if(et == ET_MouseMove)
  {
    propagateEnterLeaveEvent(event);
    previousMouseMoveStack = currentViewStack;
  }
}

void EventSystem::propagateEnterLeaveEvent(Event* event)
{
  // "leave" events for old views, "enter" events for new views
  s32 maxn = (s32)std::max(currentViewStack.size(), previousMouseMoveStack.size());
  View* oldView = NULL;
  View* newView = NULL;
  event->base.bubbles = true;
  for(s32 i=0; i<maxn; ++i)
  {
    oldView = i < previousMouseMoveStack.size() ? previousMouseMoveStack[i] : NULL;
    newView = i < currentViewStack.size() ? currentViewStack[i] : NULL;
    
    if(oldView != newView)
    {
      if(oldView)
      {
        event->base.target = oldView;
        event->base.type = ET_MouseLeave;
//        DOUT("leaving "<<oldView->name());
        propagateEvent(previousMouseMoveStack, event, i);
      }
      if(newView)
      {
        event->base.target = newView;
        event->base.type = ET_MouseEnter;
//        DOUT("entering "<<newView->name());
        propagateEvent(currentViewStack, event, i);
      }
    }
  }
}

void EventSystem::propagateUpDownEvent(Event* event)
{
  EventType et = event->base.type;
  if(et == ET_MouseDown)
  {
    if(currentViewStack.size() == 0)
    {
      WOUT("currentViewStack was empty!");
      return;
    }
  
    event->base.target = currentViewStack.back();
    propagateEvent(currentViewStack, event, (s32)currentViewStack.size()-1);
    previousMouseClickStack = currentViewStack;
  }
  else if(et == ET_MouseUp)
  {
    s32 oldIndex = previousMouseClickStack.size() > 0 ? (s32)previousMouseClickStack.size()-1 : -1;
    s32 newIndex = currentViewStack.size() > 0 ? (s32)currentViewStack.size()-1 : -1;
    View* oldView = oldIndex > 0 ? previousMouseClickStack[oldIndex] : NULL;
    View* newView = newIndex > 0 ? currentViewStack[newIndex] : NULL;
    if(oldView && (oldView != newView))
    {
      event->base.target = oldView;
      event->base.type = ET_MouseUpOutside;
      event->base.bubbles = true;
      propagateEvent(previousMouseClickStack, event, oldIndex);
      event->base.bubbles = true;
      event->base.type = ET_MouseUp;
      propagateEvent(previousMouseClickStack, event, oldIndex);
    }
    if(newView)
    {
      event->base.target = newView;
      event->base.type = ET_MouseUpInside;
      event->base.bubbles = true;
      propagateEvent(currentViewStack, event, newIndex);
      event->base.type = ET_MouseUp;
      event->base.bubbles = true;
    }
    previousMouseClickStack = currentViewStack;
  }
}

void EventSystem::propagateFocusEvent(Event* event)
{
  event->base.bubbles = true;
  event->base.stopDispatch = false;
  event->base.stopPropagation = false;
  
  currentFocusStack = currentViewStack;
  
  s32 maxn = (s32)std::max(currentFocusStack.size(), previousFocusStack.size());
  View* oldView = NULL;
  View* newView = NULL;
  
  for(s32 i=0; i<maxn; ++i)
  {
    oldView = i < previousFocusStack.size() ? previousFocusStack[i] : NULL;
    newView = i < currentFocusStack.size() ? currentFocusStack[i] : NULL;
    
    if(oldView != newView)
    {
      focusChanged = true;
      if(oldView && oldView->focusable)
      {
        oldView->focused = false;
        event->base.bubbles = true;
        event->base.target = oldView;
        event->base.type = ET_FocusLost;
        propagateEvent(previousFocusStack, event, i);
      }
      if(newView && newView->focusable)
      {
        newView->focused = true;
        event->base.bubbles = true;
        event->base.target = newView;
        event->base.type = ET_FocusGained;
        propagateEvent(currentFocusStack, event, i);
      }
    }
  }
  previousFocusStack = currentViewStack;
}

void EventSystem::loseFocus(View* view)
{
  // check if view is in focusStack and find index
  s32 viewIndex =-1;
  for(s32 i=0; i<previousFocusStack.size(); ++i)
  {
    if(previousFocusStack[i] == view)
    {
      viewIndex = i;
      break;
    }
  }
  
  // rootview can never lose focus, so we test for > 0
  if(viewIndex > 0)
  {
    // if it was, send it a FocusLost event, and all focused views underneath it.
    Event* event = Application::instance()->eventPool->borrowEvent();
    event->base.bubbles = true;
    event->base.stopDispatch = false;
    event->base.stopPropagation = false;
    event->base.type = ET_FocusLost;
    for(s32 i=viewIndex; i<previousFocusStack.size(); ++i)
    {
      View* currentView = previousFocusStack[i];
      if(currentView->focused)
      {
        currentView->focused = false;
        event->base.target = currentView;
        propagateEvent(previousFocusStack, event, i);
      }
    }
    // remove the view and all focused views underneath from the currentViewStack
    previousFocusStack.resize(viewIndex);
  }
}

void EventSystem::gainFocus(View* view)
{
  if(view && view->focusable && view->isSubviewOf(rootView))
  {
    viewStackForView(currentViewStack, view);
    Event* event = Application::instance()->eventPool->borrowEvent();
    event->base.bubbles = true;
    event->base.stopDispatch = false;
    event->base.stopPropagation = false;
    event->base.type = ET_FocusGained;
    propagateFocusEvent(event);
  }
}

View* EventSystem::focusedView()
{
  if(focusChanged)
  {
    focusChanged = false;
    currentlyFocusedView = NULL;
    for(View* view : currentFocusStack)
    {
      if(view->focused)
      {
        currentlyFocusedView = view;
      }
      else
      {
        break;
      }
    }
  }
  return currentlyFocusedView;
}

void EventSystem::propagateEvent(const ViewStack& vs, Event* event, s32 targetIndex)
{
  if(!event->base.stopPropagation) { propagateCaptureEvents(vs, event, targetIndex); }
  if(!event->base.stopPropagation) { propagateTargetEvent(vs, event, targetIndex); }
  if(!event->base.stopPropagation && event->base.bubbles) { propagateBubbleEvents(vs, event, targetIndex); }
}

void EventSystem::propagateCaptureEvents(const ViewStack& vs, Event* event, s32 targetIndex)
{
  // capture is only called on Views "before" the target,
  // i.e. in view hierarchies with only one view, no capture is called.
  for(u32 i=0; (i<targetIndex) && !event->base.stopPropagation && !event->base.stopDispatch; ++i)
  {
    View* view = vs[i];
    event->base.currentTarget = view;
    event->base.phase = EP_Capture;
    view->captureEventDispatcher.dispatchEvent(event);
  }
}

void EventSystem::propagateTargetEvent(const ViewStack& vs, Event* event, s32 targetIndex)
{
  // only called on lowermost view in the stack
  if(!event->base.stopDispatch && !event->base.stopPropagation)
  {
    View* view = vs[targetIndex];
    event->base.currentTarget = event->base.target;
    event->base.phase = EP_Target;
    view->targetEventDispatcher.dispatchEvent(event);
  }
}

void EventSystem::propagateBubbleEvents(const ViewStack& vs, Event* event, s32 targetIndex)
{
  // called on all views in the stack but the lowermost, in reverse order
  s32 i=targetIndex-1;
  while((i>=0) && !event->base.stopDispatch && !event->base.stopPropagation && event->base.bubbles)
  {
    View* view = vs[i];
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
    DOUT("-> "<<v->name());
  }
}

void EventSystem::viewStackForView(ViewStack& vs, View* view)
{
  vs.clear();
  
  while(view)
  {
    vs.push_back(view);
    view = view->superview;
  }
  
  std::reverse(vs.begin(), vs.end());
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

