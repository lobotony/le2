#ifndef LOST_LAYER_H
#define LOST_LAYER_H

#include "lost/Frame.h"

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
  void removeAllSublayers();
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
  
  void cornerRadius(s16 v);
  s16 cornerRadius();
  
  void backgroundColor(const Color& v);
  Color backgroundColor();
  
  void borderColor(const Color& v);
  Color borderColor();
  
  void borderWidth(f32 v);
  f32 borderWidth();
  
  void backgroundImage(const TexturePtr& v);
  TexturePtr backgroundImage();
  
  // hit test
  bool containsPoint(const Vec2& gp); // gp in global window coordinates
  
  
  ////////////////////////////
  string name; // for debugging only
  
  Layer* superlayer;
  vector<LayerPtr> sublayers;

private:
  s16         _cornerRadius;
  Color       _backgroundColor;
  TexturePtr  _backgroundImage;
  Color       _borderColor;
  f32         _borderWidth;

  Rect        _rect;
  bool        _visible;
};
}

#endif

