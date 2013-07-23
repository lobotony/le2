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
  View* focusedView();

  void viewDying(View* view);

  void reset(); // called when ui is disabled, clears and resets all state

  View* rootView;
  
private:
  typedef vector<View*> ViewStack;

  void propagateMouseEvent(Event* event);
  void propagateUpDownEvent(Event* event);
  void propagateEnterLeaveEvent(Event* event);
  void propagateFocusEvent(Event* event);
  void propagateKeyEvent(Event* event);
  void updateCurrentViewStack(Event* mouseEvent);
  void viewStackForView(ViewStack& vs, View* view);
  void logViewStack(const vector<View*>& vs);

  void propagateEvent(const ViewStack& vs, Event* event, s32 targetIndex);
  void propagateCaptureEvents(const ViewStack& vs, Event* event, s32 targetIndex);
  void propagateTargetEvent(const ViewStack& vs, Event* event, s32 targetIndex);
  void propagateBubbleEvents(const ViewStack& vs, Event* event, s32 targetIndex);
  
  ViewStack currentViewStack;
  ViewStack previousMouseMoveStack;
  ViewStack previousMouseClickStack;
  ViewStack currentFocusStack;
  ViewStack previousFocusStack;

  bool focusChanged;
  View* currentlyFocusedView;
};
}


#endif

