#ifndef LOST_LAYER_H
#define LOST_LAYER_H

#include "lost/Frame.h"
#include "lost/Variant.h"

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
  
  void x(f32 v);
  f32 x() const;
  
  void y(f32 v);
  f32 y() const;
  
  void width(f32 v);
  f32 width() const;
  
  void height(f32 v);
  f32 height() const;
  
  void cornerRadius(s16 v);
  s16 cornerRadius() const;
  
  void backgroundColor(const Color& v);
  Color backgroundColor() const;

  void borderColor(const Color& v);
  Color borderColor() const;
  
  void borderWidth(f32 v);
  f32 borderWidth() const;
  
  void opacity(f32 v);
  f32 opacity() const;
  
  void backgroundImage(const ImagePtr& v);
  ImagePtr backgroundImage() const;
  
  // hit test
  bool containsPoint(const Vec2& gp); // gp in global window coordinates
  
  
  // animation
  void addAnimation(const string& key, const AnimationPtr& animation);
  AnimationPtr animation(const string& key);
  void removeAnimation(const string& key);
  void removeAllAnimations();
  bool hasAnimations();
  void updateAnimations(TimeInterval now);
  void setValue(const string& key, const Variant& v, bool animated = false);
  Variant getValue(const string& key);

  
  ////////////////////////////
  string name; // for debugging only
  
  Layer* superlayer;
  vector<LayerPtr> sublayers;

private:
  s16         _cornerRadius;
  Color       _backgroundColor;
  ImagePtr    _backgroundImage;
  Color       _borderColor;
  f32         _borderWidth;
  f32         _opacity;

  Rect        _rect;
  bool        _visible;
  
  map<string, AnimationPtr> animations;
  vector<string> removeKeys;
  
  void startAnimating();
  void stopAnimating();
  void addDefaultKeyAccessors();
  void addDefaultActions();
  void safeSetValue(const string& key, const Variant& v);
  
  map<string, std::function<void(const Variant& v)>> key2setter;
  map<string, std::function<Variant()>> key2getter;
  map<string, std::function<AnimationPtr(const string&, const Variant&)>> key2action;
};
}

#endif

