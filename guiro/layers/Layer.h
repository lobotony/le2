#ifndef GUIRO_LAYER_H
#define GUIRO_LAYER_H

#include "guiro/Frame.h"
#include "lost/Color.h"
#include "guiro/types.h"

namespace lost
{

struct Layer : enable_shared_from_this<Layer>
{
  Layer();
  virtual ~Layer();
  
  void addSublayer(const LayerPtr& layer);
  void removeSublayer(const LayerPtr& layer);
  void removeFromSuperlayer();
  bool isSublayer(const LayerPtr& layer);
  
  void needsRedraw(); // invalidate texture cache in compositor, force content redraw and composition
  
  u16 z();
  string name;
  Frame frame;
  Rect  rect;
  
  string description();
  
  Color backgroundColor;
  LayerPtr superlayer;
  vector<LayerPtr> sublayers;
};
}

#endif

