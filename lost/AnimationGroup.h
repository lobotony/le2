#pragma once

#include "lost/Animation.h"

namespace lost
{

struct AnimationGroup : Animation
{

  void addAnimation(const AnimationPtr& animation);
  void removeAnimations(const AnimationPtr& animation);
  
  vector<AnimationPtr> animations;
};

}

