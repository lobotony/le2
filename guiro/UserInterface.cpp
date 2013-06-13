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

  rootView.reset(new View);
  rootView->layer->rect = Rect(100,100,100,200);
  rootView->layer->backgroundColor = redColor;
  
  LayerPtr sl1(new Layer);
  sl1->rect = Rect(50,50,200,20);
  sl1->backgroundColor = greenColor;
  
  LayerPtr sl2(new Layer);
  sl2->rect = Rect(75,100,30,40);
  sl2->backgroundColor = yellowColor;

  LayerPtr sl3(new Layer);
  sl3->rect = Rect(300,100,90,10);
  sl3->backgroundColor = blueColor;
  
  rootView->layer->addSublayer(sl1);
  rootView->layer->addSublayer(sl2);
  sl2->addSublayer(sl3);
  
  DOUT("root Z:"<<rootView->layer->z(););
  DOUT("sl1 Z:"<<sl1->z(););
  DOUT("sl2 Z:"<<sl2->z(););
  DOUT("sl3 Z:"<<sl3->z(););
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

void UserInterface::draw(Context* glContext)
{
  renderSystem->draw(glContext, rootView);
}

}
