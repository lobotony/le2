#include "lost/views/View.h"
#include "lost/Application.h"
#include "lost/UserInterface.h"

namespace lost
{

View::View()
{
  layer.reset(new Layer);
  superview = NULL;
  focusable = true;
  focused = false;
  name("View");
  _userInteractionEnabled = true;
}

View::~View()
{
  Application::instance()->ui->viewDying(this);
}

#pragma mark - hierarchy -

bool View::containsSubview(const ViewPtr& view)
{
  bool result = false;
  auto pos = std::find(subviews.begin(), subviews.end(), view);
  if(pos != subviews.end())
  {
    result = true;
  }
  return result;
}

void View::addSubview(const ViewPtr& view)
{
  if(!containsSubview(view))
  {
    if(!view->superview)
    {
      subviews.push_back(view);
      layer->addSublayer(view->layer);
      view->superview = this;
    }
    else
    {
      WOUT("tried to insert subview that already had superview");
    }
  }
  else
  {
    WOUT("tried to insert subview that is already contained in this view, ignoring");
  }
}

void View::removeSubview(const ViewPtr& view)
{
  if(containsSubview(view))
  {
    if(view->superview == this)
    {
      subviews.remove(view);
      layer->removeSublayer(view->layer);
      view->superview = NULL;
    }
    else
    {
      WOUT("tried to remove subview that was part of this view, but didn't have the correct superview, ignoring");
    }
  }
  else
  {
    WOUT("tried to remove subview that wasn't part of this view, ignoring");
  }
}

void View::removeFromSuperview()
{
  ASSERT(superview != NULL, "tried to remove subview without superview");
  superview->removeSubview(shared_from_this());
}

void View::removeAllSubviews()
{
  for(const ViewPtr& view: subviews)
  {
    removeSubview(view);
  }
}

bool View::isSubviewOf(View* root)
{
  bool result = false;
  View* view = this;
  while(view)
  {
    if(view == root)
    {
      result = true;
      break;
    }
    view = view->superview;
  }
  return result;
}

void View::composite(bool v)
{
  layer->composite(v);
}

bool View::composite()
{
  return layer->composite();
}

static u32 _numViews = 0;
void _logTree(u32 depth, View* current)
{
  string spaces;
  _numViews++;
  for(u32 i=0; i<depth;++i) { spaces += "-";}
  DOUT(spaces << current->name());
  for(auto view : current->subviews)
  {
    _logTree(depth+1, view.get());
  }
}

void View::logTree()
{
  u32 depth = 0;
  _numViews = 0;
  _logTree(depth, this);
  DOUT("num view:"<<_numViews);
}

void View::bringSubviewToFront(const ViewPtr& view)
{
  auto pos = find(subviews.begin(), subviews.end(), view);
  if(pos != subviews.end())
  {
    removeSubview(view);
    addSubview(view);
  }
}

#pragma mark - debug -

string View::name()
{
  return layer->name;
}

void View::name(const string& v)
{
  layer->name = v;
}

#pragma mark - focus handling -

void View::gainFocus()
{
  Application::instance()->ui->gainFocus(this);
}

void View::loseFocus()
{
  Application::instance()->ui->loseFocus(this);
}

#pragma mark - visibility -

bool View::isVisibleWithinSuperviews()
{
  return layer->isVisibleWithinSuperlayers();
}

void View::visible(bool val)
{
  layer->visible(val);
}

bool View::visible()
{
  return layer->visible();
}

#pragma mark - basic appearance -

void View::backgroundColor(const Color& v) { layer->backgroundColor(v); }
Color View::backgroundColor() { return layer->backgroundColor(); }

void View::borderColor(const Color& v) { layer->borderColor(v); }
Color View::borderColor() { return layer->borderColor(); }

void View::borderWidth(f32 v) { layer->borderWidth(v); }
f32 View::borderWidth() { return layer->borderWidth(); }

void View::opacity(f32 v) { layer->opacity(v); }
f32 View::opacity() { return layer->opacity(); }

void View::backgroundImage(const ImagePtr& v) { layer->backgroundImage(v); }
ImagePtr View::backgroundImage() { return layer->backgroundImage(); }

#pragma mark - hit test -

bool View::containsPoint(const Vec2& point)
{
  return layer->containsPoint(point);
}

#pragma mark - basic geometry -

void View::rect(f32 x, f32 y, f32 w, f32 h) { layer->rect(x, y, w, h); }
void View::rect(const Rect& r) { layer->rect(r); }
const Rect& View::rect() const { return layer->rect(); }
void View::centerInSuperview() { layer->centerInSuperlayer(); }

void View::pos(f32 x, f32 y) { pos(Vec2(x,y)); }
void View::pos(const Vec2& p) { layer->pos(p); }
Vec2 View::pos() const { return layer->pos(); }

void View::size(f32 w, f32 h) { size(Vec2(w,h)); }
void View::size(const Vec2& sz) { layer->size(sz); }
Vec2 View::size() const { return layer->size(); }

void View::x(f32 v) { layer->x(v); }
f32 View::x() const { return layer->x(); }

void View::y(f32 v) { layer->y(v); }
f32 View::y() const { return layer->y(); }

void View::width(f32 v) { layer->width(v); }
f32 View::width() const { return layer->width(); }

void View::height(f32 v) { layer->height(v); }
f32 View::height() const { return layer->height(); }

#pragma mark - event handing -

EDConnection View::addEventHandler(EventType et, EventHandler handler, EventPhase phase)
{
  if(phase == EP_Bubble)
  {
    return bubbleEventDispatcher.addHandler(et, handler);
  }
  else if(phase == EP_Target)
  {
    return targetEventDispatcher.addHandler(et, handler);
  }
  else if(phase == EP_Capture)
  {
    return captureEventDispatcher.addHandler(et, handler);
  }
  ASSERT(false, "unknown phase: "<<phase);
  return EDConnection();
}

void View::removeEventHandler(const EDConnection& connection)
{
}

void View::dispatchEvent(Event* event, EventPhase phase)
{
}

void View::userInteractionEnabled(bool val)
{
  _userInteractionEnabled = val;
}

bool View::userInteractionEnabled()
{
  return _userInteractionEnabled;
}


}

