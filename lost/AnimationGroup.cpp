//
//  AnimationGroup.cpp
//  LostEngine2
//
//  Created by Tony Kostanjsek on 20.07.13.
//  Copyright (c) 2013 Tony Kostanjsek. All rights reserved.
//

#include "AnimationGroup.h"

namespace lost
{
void AnimationGroup::addAnimation(const AnimationPtr& animation)
{
  animations.push_back(animation);
}

void AnimationGroup::removeAnimations(const AnimationPtr& animation)
{
  auto pos = find(animations.begin(), animations.end(), animation);
  if(pos != animations.end())
  {
    animations.erase(pos);
  }
}

}

