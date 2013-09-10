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
#include "lost/Variant.h"

namespace lost
{
struct Animation
{
  TimeInterval    beginTime;
  TimeInterval    timeOffset;
  TimeInterval    duration;
  f32             speed;

  f32             repeatCount; // 0 = ignore
  TimeInterval    repeatDuration; // 0 = ignore
  void            repeatForever(); // sets repeatCount to infinity
  bool            doesRepeatForever();

  bool            autoreverses;

  TimingFunction  timingFunction;
  
  Variant         startValue;
  Variant         endValue;
  string          key;
  
  Animation();
  virtual ~Animation();
  bool    stopped(TimeInterval now);
  f32     periodLength(); // in local time, unscaled by speed
  f32     totalDuration(); // returns total duration in local time, taking autoreverse and repeats into account. Speed is not applied yet.
  f32     currentAbsoluteTime(TimeInterval now); // returns the absolute time within the period
  Variant currentValue(TimeInterval now);
};
}

#endif /* defined(__LostEngine2__Animation__) */
