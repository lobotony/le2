#ifndef GUIRO_LAYER_H
#define GUIRO_LAYER_H

namespace lost
{

struct Layer;
typedef lost::shared_ptr<Layer> LayerPtr;

struct Layer
{
  Layer();
  virtual ~Layer();
};
}

#endif

