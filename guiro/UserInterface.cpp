#include "UserInterface.h"
#include "guiro/EventSystem.h"
#include "guiro/UpdateSystem.h"
#include "guiro/RenderSystem.h"

namespace lost
{

UserInterface::UserInterface()
{
  eventSystem = new EventSystem;
  updateSystem = new UpdateSystem;
  renderSystem = new RenderSystem;
}

UserInterface::~UserInterface()
{
  delete renderSystem;
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
      renderSystem->windowResized(Vec2(w, h));
    }
    else if(event->base.type == ET_MouseMoveEvent)
    {
    }
  }

  // FIXME: event system needs to handle any incoming events
  // FIXME: layout system needs to layout any views and layers in queue
}

void UserInterface::draw()
{
  renderSystem->draw(rootView);
}

void UserInterface::needsRedraw(Layer* layer)
{
  renderSystem->needsRedraw(layer);
}

}
