#ifndef GUIRO_LAYER_H
#define GUIRO_LAYER_H

#include "guiro/Frame.h"
#include "lost/Color.h"
#include "guiro/types.h"

namespace lost
{

struct DrawContext;

struct Layer : enable_shared_from_this<Layer>
{
  Layer();
  virtual ~Layer();
  
  void addSublayer(const LayerPtr& layer);
  void removeSublayer(const LayerPtr& layer);
  void removeFromSuperlayer();
  bool isSublayer(const LayerPtr& layer);
  bool isSublayerOf(Layer* root); // return true if root is one of the superlayers of this layer
  u16 z();
  
  
  bool isVisibleWithinSuperlayers(); // returns visibility of this and all superlayers
  void visible(bool val); // sets this layers visibility flag
  bool visible(); // returns this layers visibility flag
  
  virtual void draw(DrawContext* ctx);
  
  void needsRedraw(); // invalidate texture cache in compositor, force content redraw and composition

  virtual string description();
  
  void rect(f32 x, f32 y, f32 w, f32 h);
  void rect(const Rect& r);
  const Rect& rect() const;
  
  void pos(const Vec2& p);
  Vec2 pos() const;
  
  void size(const Vec2& sz);
  Vec2 size() const;
  
  
  string name;
  Frame frame;
  
  u16 cornerRadius;
  Color backgroundColor;
  Color borderColor;
  
  LayerPtr superlayer;
  vector<LayerPtr> sublayers;

private:
  Rect  _rect;
  bool _visible;
};
}

#endif

