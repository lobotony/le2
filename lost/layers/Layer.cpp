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
  superlayer = NULL;
  
  _cornerRadius = 0;
  _backgroundColor = whiteColor;
  _borderColor = clearColor;
  _borderWidth = 0;
  _opacity = 1.0f;
  _visible = true;
  _backgroundContentMode = LayerContentModeScaleToFill;
  addDefaultKeyAccessors();
  addDefaultActions();
  composite(false);
  needsRedraw();
}

Layer::~Layer()
{
  Application::instance()->ui->layerDying(this);
}

static u32 _numLayers = 0;
void _logTree(u32 depth, Layer* current)
{
  string spaces;
  _numLayers++;
  for(u32 i=0; i<depth;++i) { spaces += "-";}
  DOUT(spaces << current->name);
  for(auto layer : current->sublayers)
  {
    _logTree(depth+1, layer.get());
  }
}

void Layer::logTree()
{
  u32 depth = 0;
  _numLayers = 0;
  _logTree(depth, this);
  DOUT("num layers:"<<_numLayers);
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
    layer->needsRedraw();
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

void Layer::composite(bool v)
{
  _composite = v;
  needsRedraw();
}

bool Layer::composite()
{
  return _composite;
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
        Rect drawRect = calculateDrawRectFor(r, _backgroundImage, _backgroundContentMode);
        ctx->drawImage(_backgroundImage, drawRect, _backgroundColor);
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
    if(_cornerRadius > 0)
    {
      ctx->drawRoundRectFrame(Rect(0,0,_rect.size()), _cornerRadius, _borderWidth, _borderColor);
    }
    else
    {
      ctx->drawRectFrame(Rect(0,0,_rect.size()), _borderWidth, _borderColor);
    }
  }
}

Rect Layer::calculateDrawRectFor(const Rect& originalRect, const ImagePtr& img, LayerContentMode mode)
{
  Rect result = originalRect;
  
  switch(img->resizeMode)
  {
    case ImageResizeModeNinePatch:break; // do nothing since ninepatch will always span all of the original rect
    case ImageResizeModeTile:break; // do nothing, because image will be tiled acros all of the original rect
    case ImageResizeModeStretch: // adjust to content mode
    {
      switch(mode)
      {
        case LayerContentModeScaleToFill:break; // do nothing, results in original rect
        case LayerContentModeScaleAspectFit: // squeeze image into available space, preserving aspect ratio
        {
          f32 scale = 1;
          if(img->size.height > img->size.width)
          {
            scale = originalRect.height / img->size.height;
          }
          else
          {
            scale = originalRect.width / img->size.width;
          }
          result.width = floorf(img->size.width * scale);
          result.height = floorf(img->size.height * scale);
          result.centerWithin(originalRect);
          break;
        }
        case LayerContentModeScaleAspectFill: // scale image to fill available space, preserving aspect ratio
        {
          f32 scale = 1;
          if(img->size.height > img->size.width)
          {
            scale = originalRect.width / img->size.width;
          }
          else
          {
            scale = originalRect.height / img->size.height;
          }
          result.width = floorf(img->size.width * scale);
          result.height = floorf(img->size.height * scale);
          result.centerWithin(originalRect);
          break;
        }
        case LayerContentModeCenter:
        {
          result.size(img->size);
          result.centerWithin(originalRect);
          break;
        }
        default:
        {
          WOUT("don't know what to do with LayerContentMode: "<<mode);
          break;
        }
      }
      break;
    }
    default:
    {
      WOUT("don't know what to do with ImageResizeMode: "<<img->resizeMode);
      break;
    }
  }
  result.floor();
  return result;
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

void Layer::centerInSuperlayer()
{
  if(superlayer)
  {
    Rect r = _rect;
    r.centerWithin(superlayer->rect());
    r.floor();
    rect(r);
  }
}

void Layer::pos(const Vec2& p)
{
  Rect r = _rect;
  r.pos(p);
  r.floor();
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
  r.floor();
  rect(r);
}

Vec2 Layer::size() const
{
  return _rect.size();
}

void Layer::x(f32 v) { Rect r = _rect;r.x = floorf(v);rect(r); }
f32 Layer::x() const { return _rect.x; }

void Layer::y(f32 v) { Rect r = _rect;r.y = floorf(v);rect(r); }
f32 Layer::y() const { return _rect.y; }

void Layer::width(f32 v) { Rect r = _rect;r.width = floorf(v);rect(r); }
f32 Layer::width() const { return _rect.width; }

void Layer::height(f32 v) { Rect r = _rect;r.height = floorf(v);rect(r);  }
f32 Layer::height() const { return _rect.height; }

#pragma mark - Draw Properties -

void Layer::cornerRadius(s16 v) {_cornerRadius=v; needsRedraw(); }
s16 Layer::cornerRadius() const { return _cornerRadius; };

void Layer::backgroundColor(const Color& v) {_backgroundColor = v; needsRedraw(); }
Color Layer::backgroundColor() const { return _backgroundColor; };

void Layer::borderColor(const Color& v) { _borderColor=v;needsRedraw(); }
Color Layer::borderColor() const {return _borderColor; }

void Layer::borderWidth(f32 v) { _borderWidth=v; needsRedraw(); }
f32 Layer::borderWidth() const { return _borderWidth; }

void Layer::backgroundImage(const ImagePtr& v) { _backgroundImage=v; needsRedraw(); }
ImagePtr Layer::backgroundImage() const { return _backgroundImage; }

void Layer::backgroundContentMode(LayerContentMode v) { _backgroundContentMode = v; needsRedraw(); }
LayerContentMode Layer::backgroundContentMode() { return _backgroundContentMode; }

void Layer::opacity(f32 v) { _opacity=v; if(superlayer) { superlayer->needsRedraw(); } };
f32 Layer::opacity() const { return _opacity; }

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
  animation->key = key;
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

void Layer::safeSetValue(const string& key, const Variant& v)
{
  auto pos = key2setter.find(key);
  if(pos != key2setter.end())
  {
    pos->second(v);
  }
  else
  {
    EOUT("can't find setter for key "<<key);
  }
}


void Layer::updateAnimations(TimeInterval now)
{
  // run all animations if not stopped
  for(const auto& entry : animations)
  {
    const AnimationPtr& animation = entry.second;
    if(!animation->stopped(now))
    {
      safeSetValue(animation->key, animation->currentValue(now));
    }
    else
    {
      safeSetValue(animation->key, animation->endValue); // assign end value on stop to make sure end result is as expected
      if(animation->completionHandler)
      {
        animation->completionHandler(this, animation.get());
      }
      removeKeys.push_back(entry.first);
    }
  }
  
  // remove stopped animations
  for(auto key : removeKeys)
  {
//    DOUT("removing animation '"<<key<<"' from layer '"<<name<<"'");
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

  key2setter["pos"] = [this](const Variant& v)
  {
    ASSERT(v.type==VT_vec2, "pos must be Vec2");
    pos(v.vec2);
  };
  key2getter["pos"] = [this]() { return Variant(pos()); };

  key2setter["x"] = [this](const Variant& v)
  {
    ASSERT(v.type==VT_float, "x must be f32");
    x(v.f);
  };
  key2getter["x"] = [this]() { return Variant(x()); };

  key2setter["y"] = [this](const Variant& v)
  {
    ASSERT(v.type==VT_float, "y must be f32");
    y(v.f);
  };
  key2getter["y"] = [this]() { return Variant(y()); };

  key2setter["width"] = [this](const Variant& v)
  {
    ASSERT(v.type==VT_float, "width must be f32");
    width(v.f);
  };
  key2getter["width"] = [this]() { return Variant(width()); };

  key2setter["height"] = [this](const Variant& v)
  {
    ASSERT(v.type==VT_float, "height must be f32");
    height(v.f);
  };
  key2getter["height"] = [this]() { return Variant(height()); };
}

void Layer::addDefaultActions()
{
  auto standardAction = [this](const string& key, const Variant& endValue)
  {
    AnimationPtr result(new Animation);
    f32 duration = .5;
    f32 speed = 1;
    
    result->beginTime = currentTimeSeconds();
    result->duration = duration;
    result->speed = speed;
    result->startValue = getValue(key);
    result->endValue = endValue;
    result->timingFunction = TimingFunction::easeOut();
    result->key = key;
    
    return result;
  };

  key2action["pos"] = standardAction;
  key2action["size"] = standardAction;
  key2action["x"] = standardAction;
  key2action["y"] = standardAction;
  key2action["width"] = standardAction;
  key2action["height"] = standardAction;
  key2action["opacity"] = standardAction;
}

void Layer::setValue(const string& key, const Variant& v, bool animated)
{
  bool setWithoutAnimation = false;
  if(animated)
  {
    auto pos = key2action.find(key);
    if(pos != key2action.end())
    {
      addAnimation(key, pos->second(key, v));
    }
    else
    {
      WOUT("no animation found for key: "<<key);
      setWithoutAnimation = true;
    }
  }
  
  if(!animated or setWithoutAnimation)
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
}

Variant Layer::getValue(const string& key)
{
  auto pos = key2getter.find(key);
  ASSERT(pos != key2getter.end(), "can't find getter for key "<<key);
  return pos->second();
}

}


