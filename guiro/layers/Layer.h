#ifndef GUIRO_LAYER_H
#define GUIRO_LAYER_H

#include "guiro/Frame.h"

namespace lost
{

struct Layer;
typedef lost::shared_ptr<Layer> LayerPtr;

struct Layer
{
  Layer();
  virtual ~Layer();
  
  Frame frame;
  Rect  rect;
};
}

#endif

