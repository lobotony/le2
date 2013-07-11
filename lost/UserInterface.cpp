#include "lost/UserInterface.h"
#include "lost/EventSystem.h"
#include "lost/UpdateSystem.h"
#include "lost/Compositor.h"
#include "lost/Application.h"

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

void UserInterface::processEvents(const EventQueue::Container& events)
{
  if(!rootView) return; // bail immediately if ui is disabled

  for(Event* event : events)
  {
    if(event->base.type == ET_WindowResize)
    {
      windowResized(Application::instance()->windowSize);
    }
    eventSystem->propagateEvent(event);
  }

  // FIXME: layout system needs to layout any views and layers in queue
}

void UserInterface::windowResized(const Vec2& sz)
{
  if(rootView)
  {
    rootView->layer->rect(Rect(0,0,sz));
  }
  compositor->windowResized(sz);
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
    rootView->name = "rootView";
    rootView->layer->backgroundColor(clearColor);
    windowResized(Application::instance()->windowSize);
    eventSystem->rootView = rootView.get();
  }
}

void UserInterface::disable()
{
  rootView.reset();
  eventSystem->rootView = NULL;
}

}
