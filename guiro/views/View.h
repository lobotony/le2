#ifndef GUIRO_VIEW_H
#define GUIRO_VIEW_H

#include "guiro/Frame.h"
#include "lost/Rect.h"
#include "guiro/layers/Layer.h"

namespace lost
{

struct View : enable_shared_from_this<View>
{
  View();
  virtual ~View();
  
  bool containsSubview(const ViewPtr& view); // does NOT recurse
  void addSubview(const ViewPtr& view);
  void removeSubview(const ViewPtr& view);
  void removeFromSuperview();
  
  Rect rect;
  Frame frame;
  LayerPtr layer;
  View* superview;
  lost::list<ViewPtr> subviews;
  
};

}

#endif

