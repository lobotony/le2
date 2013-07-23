#include "lost/UserInterface.h"
#include "lost/EventSystem.h"
#include "lost/Compositor.h"
#include "lost/Application.h"
#include "lost/AnimationSystem.h"

namespace lost
{

UserInterface::UserInterface()
{
  eventSystem = new EventSystem;
  compositor = new Compositor;
  animator = new AnimationSystem();
}

UserInterface::~UserInterface()
{
  delete compositor;
  delete eventSystem;
  delete animator;
}

void UserInterface::update(const EventQueue::Container& events)
{
  processEvents(events);
  animator->update();
  draw();
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
    eventSystem->rootView = rootView.get();

    rootView->name("rootView");
    rootView->layer->backgroundColor(clearColor);
    rootView->focusable = true;
    rootView->gainFocus();
    
    windowResized(Application::instance()->windowSize);
  }
}

void UserInterface::disable()
{
  if(rootView)
  {
    rootView.reset();
    eventSystem->rootView = NULL;
    eventSystem->reset();
    compositor->reset();
  }
}

void UserInterface::viewDying(View* view)
{
  eventSystem->viewDying(view);
}

void UserInterface::layerDying(Layer* layer)
{
  compositor->layerDying(layer);
  animator->stopAnimating(layer);
  animator->reset();
}

void UserInterface::gainFocus(View* view)
{
  eventSystem->gainFocus(view);
}

void UserInterface::loseFocus(View* view)
{
  eventSystem->loseFocus(view);
}

View* UserInterface::focusedView()
{
  return eventSystem->focusedView();
}

void UserInterface::startAnimating(Layer* layer)
{
  animator->startAnimating(layer);
}

void UserInterface::stopAnimating(Layer* layer)
{
  animator->stopAnimating(layer);
}

}
