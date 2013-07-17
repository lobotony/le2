#ifndef LOST_EVENTYSTEM_H
#define LOST_EVENTYSTEM_H

#include "lost/Event.h"

namespace lost
{

struct EventSystem
{
  EventSystem();
  ~EventSystem();
  
  void propagateEvent(Event* event);

  View* rootView;
  
private:
  void propagateMouseEvent(Event* event);
  void updateCurrentViewStack(Event* mouseEvent);
  void logViewStack(const vector<View*>& vs);

  void propagateEvent(Event* event, s32 targetIndex);
  void propagateCaptureEvents(Event* event, s32 targetIndex);
  void propagateTargetEvent(Event* event, s32 targetIndex);
  void propagateBubbleEvents(Event* event, s32 targetIndex);
  
  vector<View*> currentViewStack;
  vector<View*> previousMouseMoveStack;
  vector<View*> previousMouseClickStack;
  vector<View*> previousFocusStack;
  bool focusChanged;
  View* currentlyFocusedView;
};
}


#endif

