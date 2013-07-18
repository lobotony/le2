#ifndef LOST_UI_H
#define LOST_UI_H

#include "lost/views/View.h"
#include "lost/EventQueue.h"

namespace lost
{

struct Context;
struct EventSystem;
struct Compositor;

/** Main class for everything user interface related.
 * Every application has a UserInterface instance, but it is not enabled by default,
 * i.e. there is no rootView, there are no Views that take up memory, and no event handling will happen.
 * In order to actually use the UserInterface, you have to call enable(). This will create the rootView
 * and keep its size synced with the main window. Following that, you can simply add your controls to the
 * rootView. 
 * Calling disable() gets rid of the rootView again. 
 */
struct UserInterface
{
  UserInterface();
  ~UserInterface();
  
  void enable(); // creates an empty transparent rootView and keeps it stretched to main window size
  void disable(); // deletes the rootView
  
  // called by engine for basic updating and rendering
  void processEvents(const EventQueue::Container& events);
  void draw();
  
  // helper methods for views/layers so they don't need to access low level systems directly
  void needsRedraw(Layer* layer);
  
  EventSystem* eventSystem;
  Compositor* compositor;
  
  ViewPtr rootView;
  
  void windowResized(const Vec2& sz);

  // Layer/View internal hooks
  void viewDying(View* view);
  void layerDying(Layer* layer);
  void gainFocus(View* view);
  void loseFocus(View* view);
};
}

#endif
