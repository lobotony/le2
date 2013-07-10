#ifndef LOST_VIEW_H
#define LOST_VIEW_H

#include "lost/Frame.h"
#include "lost/layers/Layer.h"
#include "lost/EventDispatcher.h"

namespace lost
{

struct View : enable_shared_from_this<View>, EventDispatcher
{
  View();
  virtual ~View();
  
  bool containsSubview(const ViewPtr& view); // does NOT recurse
  void addSubview(const ViewPtr& view);
  void removeSubview(const ViewPtr& view);
  void removeFromSuperview();
  
  LayerPtr layer;
  View* superview;
  lost::list<ViewPtr> subviews;  
};

}

#endif

