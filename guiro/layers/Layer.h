#ifndef GUIRO_LAYER_H
#define GUIRO_LAYER_H

#include "guiro/Frame.h"
#include "lost/Color.h"

namespace lost
{

struct Layer;
typedef lost::shared_ptr<Layer> LayerPtr;

struct Layer : enable_shared_from_this<Layer>
{
  Layer();
  virtual ~Layer();
  
  Frame frame;
  Rect  rect;
  
  Color backgroundColor;
};
}

#endif

