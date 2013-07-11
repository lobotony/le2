#include "lost/EventSystem.h"
#include "lost/views/View.h"

namespace lost
{
EventSystem::EventSystem()
{
}
  
EventSystem::~EventSystem()
{
}

vector<View*> EventSystem::findViewStack(Event* event)
{
  vector<View*> viewStack;
  
  Vec2 pos(event->mouseEvent.x, event->mouseEvent.y);
  bool containsPoint = rootView->containsPoint(pos);
  View* view = rootView;
  
  while(containsPoint)
  {
    viewStack.push_back(view);
    containsPoint = false;

    for(auto i=view->subviews.rbegin(); i!=view->subviews.rend(); ++i)
    {
      View* v = (*i).get();
      if(v->isVisibleWithinSuperviews() && v->containsPoint(pos))
      {
        view = v;
        containsPoint = true;
        break;
      }
    }
  }
  
  return viewStack;
}

}

