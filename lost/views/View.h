#ifndef LOST_VIEW_H
#define LOST_VIEW_H

#include "lost/Frame.h"
#include "lost/layers/Layer.h"
#include "lost/EventDispatcher.h"

namespace lost
{

struct View : enable_shared_from_this<View>
{
  View();
  virtual ~View();
  
  bool containsSubview(const ViewPtr& view); // does NOT recurse
  void addSubview(const ViewPtr& view);
  void removeSubview(const ViewPtr& view);
  void removeFromSuperview();
  void removeAllSubviews();
  bool isSubviewOf(View* root);
  
  void composite(bool v);
  bool composite();
  
  // visibility
  bool isVisibleWithinSuperviews(); // returns visibility of this and all superlayers
  void visible(bool val); // sets this layers visibility flag
  bool visible(); // returns this layers visibility flag

  // basic geometry
  void rect(f32 x, f32 y, f32 w, f32 h);
  virtual void rect(const Rect& r);
  const Rect& rect() const;
  
  void pos(f32 x, f32 y);
  void pos(const Vec2& p);
  Vec2 pos() const;
  
  void size(f32 w, f32 h);
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

  string name();
  void name(const string& v);

  // basic appearance
  void backgroundColor(const Color& v);
  Color backgroundColor();

  void borderColor(const Color& v);
  Color borderColor();
  
  void borderWidth(f32 v);
  f32 borderWidth();
  
  void opacity(f32 v);
  f32 opacity();
  
  void backgroundImage(const ImagePtr& v);
  ImagePtr backgroundImage();
  
  // hit test
  bool containsPoint(const Vec2& point);
  
  void userInteractionEnabled(bool val);
  bool userInteractionEnabled();
  
  // focus handling
  void gainFocus();
  void loseFocus();
  bool focusable; // set this to true if you want this View to receive focus and focus events
  bool focused; // will be set by EventSystem, should not be set otherwise
  
  // event handling
  EDConnection addEventHandler(EventType et, EventHandler handler, EventPhase phase = EP_Bubble);
  void removeEventHandler(const EDConnection& connection);
  void dispatchEvent(Event* event, EventPhase phase);
  
  LayerPtr layer;
  View* superview;
  lost::list<ViewPtr> subviews;
  
  EventDispatcher captureEventDispatcher;
  EventDispatcher targetEventDispatcher;
  EventDispatcher bubbleEventDispatcher;

  void logTree();
  
private:
  bool _userInteractionEnabled;
};

}

#endif

