#ifndef LOST_EVENTYSTEM_H
#define LOST_EVENTYSTEM_H

#include "lost/Event.h"

namespace lost
{

struct EventSystem
{
  EventSystem();
  ~EventSystem();
  
  
  vector<View*> findViewStack(Event* mouseEvent);
  
  View* rootView;
  vector<View*> previousMouseMoveStack;
  vector<View*> previousMouseClickStack;
  vector<View*> previousFocusStack;
  bool focusChanged;
  View* currentlyFocusedView;
};
}


#endif

