//
//  AnimationGroup.h
//  LostEngine2
//
//  Created by Tony Kostanjsek on 20.07.13.
//  Copyright (c) 2013 Tony Kostanjsek. All rights reserved.
//

#ifndef __LostEngine2__AnimationGroup__
#define __LostEngine2__AnimationGroup__

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

#endif /* defined(__LostEngine2__AnimationGroup__) */
