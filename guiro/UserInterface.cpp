#include "UserInterface.h"

namespace lost
{

UserInterface::UserInterface()
{
  rootView.reset(new View);
  rootView->layer->rect = Rect(100,100,100,200);
  rootView->layer->backgroundColor = redColor;
}

UserInterface::~UserInterface()
{
}

void UserInterface::update(const EventQueue::Container& events)
{
  for(Event* event : events)
  {
    if(event->base.type == ET_WindowResize)
    {
      f32 w = event->windowResizeEvent.width;
      f32 h = event->windowResizeEvent.height;
      renderSystem.windowResized(Vec2(w, h));
    }
    else if(event->base.type == ET_MouseMoveEvent)
    {
    }
  }

  // FIXME: event system needs to handle any incoming events
  // FIXME: layout system needs to layout any views and layers in queue
}

void UserInterface::draw(Context* glContext)
{
  renderSystem.draw(glContext, rootView);
}

}
