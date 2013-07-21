//
//  Animation.h
//  LostEngine2
//
//  Created by Tony Kostanjsek on 19.07.13.
//  Copyright (c) 2013 Tony Kostanjsek. All rights reserved.
//

#ifndef __LostEngine2__Animation__
#define __LostEngine2__Animation__

#include "lost/TimingFunction.h"

namespace lost
{
struct Animation
{
  void update();

  TimeInterval  beginTime;
  TimeInterval  timeOffset;
  u32           repeatCount;
  TimeInterval  repeatDuration;
  TimeInterval  duration;
  f32           speed;
  bool          autoreverses;
  TimingFunction timingFunction;
};
}

#endif /* defined(__LostEngine2__Animation__) */
