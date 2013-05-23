#ifndef GUIRO_VIEW_H
#define GUIRO_VIEW_H

#include "guiro/Frame.h"
#include "lost/Rect.h"
#include "guiro/layers/Layer.h"

namespace lost
{

struct View;
typedef lost::shared_ptr<View> ViewPtr;

struct View : enable_shared_from_this<View>
{
  View();
  virtual ~View();
  
  bool containsSubview(ViewPtr view); // does NOT recurse
  void addSubview(ViewPtr view);
  void removeSubview(ViewPtr view);
  void removeFromSuperview();
  
  Rect rect;
  Frame frame;
  LayerPtr layer;
  View* superview;
  lost::list<ViewPtr> subviews;
  
};

}

#endif

