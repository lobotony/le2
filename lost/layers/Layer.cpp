#include "lost/layers/Layer.h"
#include "lost/UserInterface.h"
#include "lost/Application.h"
#include "lost/DrawContext.h"
#include "lost/Context.h"
#include "lost/Animation.h"

namespace lost
{

Layer::Layer()
{
  name = "";
  
  superlayer = NULL;
  
  _cornerRadius = 0;
  _backgroundColor = whiteColor;
  _borderColor = clearColor;
  _borderWidth = 0;
  _opacity = 1.0f;

  _visible = true;
  addDefaultKeyAccessors();
  needsRedraw();
}

Layer::~Layer()
{
  Application::instance()->ui->layerDying(this);
}

string Layer::description()
{
  StringStream os;
  os << "[Layer "<<(u64)this<<" '"<<name<<"']";
  return os.str();
}

#pragma mark - Layer Hierarchy -

void Layer::addSublayer(const LayerPtr& layer)
{
  if(!isSublayer(layer))
  {
    if(layer->superlayer)
    {
      WOUT("added layer that already had superlayer: "<<layer->description());
    }
    layer->superlayer = this;
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
    sublayer->superlayer = NULL;
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

void Layer::removeAllSublayers()
{
  for(const LayerPtr& layer : sublayers)
  {
    removeSublayer(layer);
  }
}

bool Layer::isSublayer(const LayerPtr& layer)
{
  return (find(sublayers.begin(), sublayers.end(), layer) != sublayers.end());
}

bool Layer::isSublayerOf(Layer* root)
{
  bool result = this == root ? true : false;
  
  Layer* l = this->superlayer;
  while(l)
  {
    if(l == root)
    {
      result = true;
      break;
    }
    l = l->superlayer;
  }
  
  return result;
}

u16 Layer::z()
{
  u16 result = 0;
  Layer* current = superlayer;
  while(current)
  {
    ++result;
    current = current->superlayer;
  }
  return result;
}

#pragma mark - Visibility -

bool Layer::isVisibleWithinSuperlayers()
{
  bool result = visible();
  
  Layer* l = this;
  while (result && l)
  {
    result = l->visible();
    l = l->superlayer;
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

#pragma mark - Drawing -

void Layer::needsRedraw()
{
  Application::instance()->ui->needsRedraw(this);
}

void Layer::draw(DrawContext* ctx)
{
  // clear buffer in any case
  ctx->glContext->clearColor(Color(0,0,0,0));
  ctx->glContext->clear(GL_COLOR_BUFFER_BIT);

  // draw background if not clear color
  if(_backgroundColor != clearColor)
  {
//    ctx->drawSolidRect(Rect(0,0,_rect.size()), Color(1,0,0,.3));
    s16 effectiveCornerRadius = _cornerRadius - _borderWidth;

    if(effectiveCornerRadius <= 0)
    {
//      DOUT("solid "<<_cornerRadius << " : " << _borderWidth);
      Rect r(0,0,_rect.size());
      if(!_backgroundImage)
      {
        ctx->drawSolidRect(r, _backgroundColor);
      }
      else
      {
        ctx->drawTexturedRect(r, _backgroundImage, _backgroundColor, false, true);
      }
    }
    else
    {
      ctx->drawRoundRect(Rect(_borderWidth,
                              _borderWidth,
                              _rect.width-2*_borderWidth,
                              _rect.height-2*_borderWidth),
                         effectiveCornerRadius,
                         _backgroundColor);
    }
  }
  if((_borderColor != clearColor) && (_borderWidth > 0))
  {
    ctx->drawRoundRectFrame(Rect(0,0,_rect.size()), _cornerRadius, _borderWidth, _borderColor);
  }
}

#pragma mark - Basic Geometry -

void Layer::rect(f32 x, f32 y, f32 w, f32 h)
{
  rect(Rect(x,y,w,h));
}

void Layer::rect(const Rect& r)
{
  if(r != _rect)
  {
    if (r.size() != _rect.size())
    {
      needsRedraw();
    }
    if(superlayer)
    {
      superlayer->needsRedraw();
    }
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

#pragma mark - Draw Properties -

void Layer::cornerRadius(s16 v) {_cornerRadius=v; needsRedraw(); }
s16 Layer::cornerRadius() { return _cornerRadius; };

void Layer::backgroundColor(const Color& v) {_backgroundColor = v; needsRedraw(); }
Color Layer::backgroundColor() { return _backgroundColor; };

void Layer::borderColor(const Color& v) { _borderColor=v;needsRedraw(); }
Color Layer::borderColor() {return _borderColor; }

void Layer::borderWidth(f32 v) { _borderWidth=v; needsRedraw(); }
f32 Layer::borderWidth() { return _borderWidth; }

void Layer::backgroundImage(const TexturePtr& v) { _backgroundImage=v; needsRedraw(); }
TexturePtr Layer::backgroundImage() { return _backgroundImage; }

void Layer::opacity(f32 v) { _opacity=v; if(superlayer) { superlayer->needsRedraw(); } };
f32 Layer::opacity() {return _opacity; }


#pragma mark - hit test -

bool Layer::containsPoint(const Vec2& gp)
{
  Vec2 p = _rect.pos();
  Layer* l = superlayer;
  while(l)
  {
    p += l->_rect.pos();
    l = l->superlayer;
  }
  Rect r(p, _rect.size());
  return r.contains(gp);
}

#pragma mark - Animation -

void Layer::addAnimation(const string& key, const AnimationPtr& animation)
{
  animations[key] = animation;
  startAnimating();
}

AnimationPtr Layer::animation(const string& key)
{
  return animations[key];
}

void Layer::removeAnimation(const string& key)
{
  auto pos = animations.find(key);
  if(pos != animations.end())
  {
    animations.erase(pos);
    if(animations.size() == 0)
    {
      stopAnimating();
    }
  }
}

void Layer::removeAllAnimations()
{
  animations.clear();
  stopAnimating();
}

bool Layer::hasAnimations()
{
  return animations.size() > 0;
}

void Layer::startAnimating()
{
  Application::instance()->ui->startAnimating(this);
}

void Layer::stopAnimating()
{
  Application::instance()->ui->stopAnimating(this);
}

void Layer::updateAnimations(TimeInterval now)
{
  // run all animations if not stopped
  for(const auto& entry : animations)
  {
    const AnimationPtr& animation = entry.second;
    if(!animation->stopped(now))
    {
      auto pos = key2setter.find(animation->key);
      if(pos != key2setter.end())
      {
        pos->second(animation->currentValue(now));
      }
      else
      {
        EOUT("can't find setter for animation key "<<animation->key);
      }
      
    }
    else
    {
      removeKeys.push_back(entry.first);
    }
  }
  
  // remove stopped animations
  for(auto key : removeKeys)
  {
    DOUT("removing animation '"<<key<<"' from layer '"<<name<<"'");
    animations.erase(key);
  }
  removeKeys.clear();
}

void Layer::addDefaultKeyAccessors()
{
  key2setter["opacity"] = [this](const Variant& v)
  {
    ASSERT(v.type==VT_float, "opacity must be float");
    opacity(v.f);
  };
  
  key2getter["opacity"] = [this]() { return Variant(_opacity); };

  key2setter["size"] = [this](const Variant& v)
  {
    ASSERT(v.type==VT_vec2, "size must be Vec2");
    size(v.vec2);
  };
  
  key2getter["size"] = [this]() { return Variant(size()); };

}

void Layer::setValue(const string& key, const Variant& v)
{
  auto pos = key2setter.find(key);
  if(pos != key2setter.end())
  {
    pos->second(v);
  }
  else
  {
    WOUT("couldn't find setter for key '"<<key<<"'");
  }
}

Variant Layer::getValue(const string& key)
{
  auto pos = key2getter.find(key);
  ASSERT(pos != key2getter.end(), "can't find getter for key "<<key);
  return pos->second();
}


}


