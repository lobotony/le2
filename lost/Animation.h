#pragma once

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
  
  std::function<void(Layer*, Animation*)> completionHandler;
};
}

