#pragma once

#include "lost/Application.h"
#include "lost/Event.h"

namespace lost
{

struct RpiDemo : Application
{
  RpiDemo();

  void startup();
  void update();
  void shutdown();
  
  void toggleSpecs();
  void toggleTouchDemo();
  
  f32 descHeight;
  ViewPtr descContainer;
  
  // specAnim
  AnimationPtr specAnim;
  bool slidingIn;
  
  // touch demo
  ViewPtr touchView;

  std::function<void(Event*)> downHandler;
  std::function<void(Event*)> moveHandler;
  std::function<void(Event*)> upHandler;
  
  bool clickedView;
  Vec2 offset;
  Vec2 areaSize;
  f32 psize;
};
  
}

