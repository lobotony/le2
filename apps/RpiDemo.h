#ifndef LOST_RPIDEMO_H
#define LOST_RPIDEMO_H

#include "lost/Application.h"

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
  
};
  
}

#endif
