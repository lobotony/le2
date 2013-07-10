#include "lost/views/View.h"
#include <algorithm>

namespace lost
{

View::View()
{
  layer.reset(new Layer);
  superview = NULL;
}

View::~View()
{
}

bool View::containsSubview(const ViewPtr& view)
{
  bool result = false;
  auto pos = std::find(subviews.begin(), subviews.end(), view);
  if(pos != subviews.end())
  {
    result = true;
  }
  return result;
}

void View::addSubview(const ViewPtr& view)
{
  if(!containsSubview(view))
  {
    if(!view->superview)
    {
      subviews.push_back(view);
      view->superview = this;
    }
    else
    {
      WOUT("tried to insert subview that already had superview");
    }
  }
  else
  {
    WOUT("tried to insert subview that is already contained in this view, ignoring");
  }
}

void View::removeSubview(const ViewPtr& view)
{
  if(containsSubview(view))
  {
    if(view->superview == this)
    {
      subviews.remove(view);
      view->superview = NULL;
    }
    else
    {
      WOUT("tried to remove subview that was part of this view, but didn't have the correct superview, ignoring");
    }
  }
  else
  {
    WOUT("tried to remove subview that wasn't part of this view, ignoring");
  }
}

void View::removeFromSuperview()
{
  ASSERT(superview != NULL, "tried to remove subview without superview");
  superview->removeSubview(shared_from_this());
}

}

