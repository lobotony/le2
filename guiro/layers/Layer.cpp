#include "guiro/layers/Layer.h"
#include "guiro/UserInterface.h"

namespace lost
{

Layer::Layer()
{
  backgroundColor = whiteColor;
}

Layer::~Layer()
{
}

string Layer::description()
{
  StringStream os;
  os << "[Layer "<<(u64)this<<" '"<<name<<"']";
  return os.str();
}

void Layer::addSublayer(const LayerPtr& layer)
{
  if(!isSublayer(layer))
  {
    if(layer->superlayer)
    {
      WOUT("added layer that already had superlayer: "<<layer->description());
    }
    layer->superlayer = shared_from_this();
    sublayers.push_back(layer);
  }
  else
  {
    WOUT("tried to add layer that was already sublayer: "<<layer->description());
  }
}

void Layer::removeSublayer(const LayerPtr& layer)
{
  auto pos = find(sublayers.begin(), sublayers.end(), layer);
  if(pos != sublayers.end())
  {
    LayerPtr sublayer = *pos;
    sublayer->superlayer.reset();
    sublayers.erase(pos);
  }
  else
  {
    WOUT("tried to remove layer that wasn't sublayer: "<<layer->description());
  }
}

void Layer::removeFromSuperlayer()
{
  if(superlayer)
  {
    superlayer->removeSublayer(shared_from_this());
  }
  else
  {
    WOUT("called removeFromSuperlayer() on layer without superlayer: "<<description());
  }
}

bool Layer::isSublayer(const LayerPtr& layer)
{
  return (find(sublayers.begin(), sublayers.end(), layer) != sublayers.end());
}

u16 Layer::z()
{
  u16 result = 0;
  LayerPtr current = superlayer;
  while(current)
  {
    ++result;
    current = current->superlayer;
  }
  return result;
}

}

