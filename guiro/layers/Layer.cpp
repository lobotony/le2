#include "guiro/layers/Layer.h"
#include "guiro/UserInterface.h"
#include "lost/Application.h"
#include "guiro/RenderContext.h"
#include "lost/Context.h"
#include <algorithm>

namespace lost
{

Layer::Layer()
{
  backgroundColor = clearColor;
  _visible = true;
  needsRedraw();
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
  auto pos = std::find(sublayers.begin(), sublayers.end(), layer);
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

void Layer::needsRedraw()
{
  Application::instance()->ui->needsRedraw(this);
}

bool Layer::isVisibleWithinSuperlayers()
{
  bool result = visible();
  
  Layer* l = this;
  while (result && l)
  {
    result = l->visible();
    l = l->superlayer.get();
  }
  
  return result;
}

void Layer::visible(bool val)
{
  _visible = val;
}

bool Layer::visible()
{
  return _visible;
}

bool Layer::isSublayerOf(Layer* root)
{
  bool result = this == root ? true : false;
  
  Layer* l = this->superlayer.get();
  while(l)
  {
    if(l == root)
    {
      result = true;
      break;
    }
    l = l->superlayer.get();
  }
  
  return result;
}

void Layer::draw(RenderContext* rc)
{
  rc->glContext->clearColor(Color(0,0,0,0));
  rc->glContext->clear(GL_COLOR_BUFFER_BIT);
  if(backgroundColor != clearColor)
  {
    rc->drawSolidRect(Rect(0,0,_rect.size()), Color(.3, 0, 0, 1));
    rc->drawRoundRect(Rect(0,0,_rect.size()), 8, backgroundColor);
  }
}

void Layer::rect(f32 x, f32 y, f32 w, f32 h)
{
  rect(Rect(x,y,w,h));
}


void Layer::rect(const Rect& r)
{
  if(r != _rect)
  {
    needsRedraw();
  }
  _rect = r;
}

const Rect& Layer::rect() const
{
  return _rect;
}

void Layer::pos(const Vec2& p)
{
  Rect r = _rect;
  r.pos(p);
  rect(r);
}

Vec2 Layer::pos() const
{
  return _rect.pos();
}

void Layer::size(const Vec2& sz)
{
  Rect r = _rect;
  r.size(sz);
  rect(r);
}

Vec2 Layer::size() const
{
  return _rect.size();
}

}


