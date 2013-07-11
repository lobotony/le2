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
  
  
  vector<View*> findViewStack(Event* mouseEvent);
  void logViewStack(const vector<View*>& vs);
  
  View* rootView;
  vector<View*> previousMouseMoveStack;
  vector<View*> previousMouseClickStack;
  vector<View*> previousFocusStack;
  bool focusChanged;
  View* currentlyFocusedView;
};
}


#endif

