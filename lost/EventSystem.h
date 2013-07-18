#ifndef LOST_EVENTYSTEM_H
#define LOST_EVENTYSTEM_H

#include "lost/Event.h"

namespace lost
{

// rootView must not be accessed in constructor since it is set later on, after all subsystem have been created
struct EventSystem
{
  EventSystem();
  ~EventSystem();
  
  void propagateEvent(Event* event);
  void loseFocus(View* view);
  void gainFocus(View* view);

  void viewDying(View* view);

  void reset(); // called when ui is disabled, clears and resets all state

  View* rootView;
  
private:
  void propagateMouseEvent(Event* event);
  void propagateUpDownEvent(Event* event);
  void propagateFocusEvent(Event* event);
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

