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
  
  // visibility
  bool isVisibleWithinSuperviews(); // returns visibility of this and all superlayers
  void visible(bool val); // sets this layers visibility flag
  bool visible(); // returns this layers visibility flag

  // basic geometry
  void rect(f32 x, f32 y, f32 w, f32 h);
  void rect(const Rect& r);
  const Rect& rect() const;
  
  void pos(const Vec2& p);
  Vec2 pos() const;
  
  void size(const Vec2& sz);
  Vec2 size() const;
  
  // hit test
  bool containsPoint(const Vec2& point);
  
  // event handling
  EDConnection addEventHandler(EventType et, EventHandler handler, EventPhase phase = EP_Bubble);
  void removeEventHandler(const EDConnection& connection);
  void dispatchEvent(Event* event, EventPhase phase);
  
  LayerPtr layer;
  View* superview;
  lost::list<ViewPtr> subviews;
  string name;
  
  EventDispatcher captureEventDispatcher;
  EventDispatcher targetEventDispatcher;
  EventDispatcher bubbleEventDispatcher;
};

}

#endif

