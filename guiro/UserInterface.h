#ifndef GUIRO_UI_H
#define GUIRO_UI_H

#include "guiro/views/View.h"
#include "lost/EventQueue.h"

namespace lost
{

struct Context;
struct EventSystem;
struct UpdateSystem;
struct RenderSystem;

struct UserInterface
{
  UserInterface();
  ~UserInterface();
  
  // called by engine for basic updating and rendering
  void update(const EventQueue::Container& events);
  void draw(Context* glContext);
  
  // helper methods for views/layers so they don't need to access low level systems directly  
  
  EventSystem* eventSystem;
  UpdateSystem* updateSystem;
  RenderSystem* renderSystem;
  
  ViewPtr rootView;
};
}

#endif
