#ifndef GUIRO_UI_H
#define GUIRO_UI_H

#include "guiro/views/View.h"
#include "guiro/EventSystem.h"
#include "guiro/UpdateSystem.h"
#include "guiro/RenderSystem.h"
#include "lost/EventQueue.h"

namespace lost
{

struct Context;

struct UserInterface
{
  UserInterface();
  ~UserInterface();
  
  void update(const EventQueue::Container& events);
  void draw(Context* glContext);
  
  EventSystem eventSystem;
  UpdateSystem updateSystem;
  RenderSystem renderSystem;
  
  ViewPtr rootView;
};
}

#endif
