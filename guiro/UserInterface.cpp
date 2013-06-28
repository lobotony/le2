#include "UserInterface.h"
#include "guiro/EventSystem.h"
#include "guiro/UpdateSystem.h"
#include "guiro/Compositor.h"

namespace lost
{

UserInterface::UserInterface()
{
  eventSystem = new EventSystem;
  updateSystem = new UpdateSystem;
  compositor = new Compositor;
}

UserInterface::~UserInterface()
{
  delete compositor;
  delete updateSystem;
  delete eventSystem;
}

void UserInterface::update(const EventQueue::Container& events)
{
  for(Event* event : events)
  {
    if(event->base.type == ET_WindowResize)
    {
      f32 w = event->windowResizeEvent.width;
      f32 h = event->windowResizeEvent.height;
      windowResized(w, h);
    }
    else if(event->base.type == ET_MouseMoveEvent)
    {
    }
  }

  // FIXME: event system needs to handle any incoming events
  // FIXME: layout system needs to layout any views and layers in queue
}

void UserInterface::windowResized(f32 w, f32 h)
{
  if(rootView)
  {
    rootView->layer->rect(Rect(0,0,w,h));
  }
  compositor->windowResized(Vec2(w, h));
}

void UserInterface::draw()
{
  if(rootView)
  {
    compositor->draw(rootView->layer);
  }
}

void UserInterface::needsRedraw(Layer* layer)
{
  compositor->needsRedraw(layer);
}

void UserInterface::enable()
{
  if(!rootView)
  {
    rootView.reset(new View);
    rootView->layer->rect(0,0,1,1); // any size other than 0,0 to prevent unnecessary OpenGL framebuffer errors
  }
}

void UserInterface::disable()
{
  rootView.reset();
}

}
