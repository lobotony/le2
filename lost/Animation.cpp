//
//  Animation.cpp
//  LostEngine2
//
//  Created by Tony Kostanjsek on 19.07.13.
//  Copyright (c) 2013 Tony Kostanjsek. All rights reserved.
//

#include "Animation.h"
#include "lost/PlatformTime.h"
#include <limits>
#include <algorithm>

using namespace std;

namespace lost
{

Animation::Animation() : timingFunction(TimingFunction::defaultFunc())
{
  beginTime = currentTimeSeconds(); // starts now
  timeOffset = 0; // without delay
  duration = 1; // 1 second length
  speed = 1; // no speed scaling, 1 = original speed

  // no repeat
  repeatCount = 0;
  repeatDuration = 0;
  
  autoreverses = false;
  
  // default timing function
}

Animation::~Animation()
{
}

f32 Animation::periodLength()
{
  f32 result = 0.0f;
  f32 reverseFactor = autoreverses ? 2.0f : 1.0f;
  result = duration*reverseFactor;
  return result;
}

f32 Animation::totalDuration()
{
  f32 repeatFactor = (repeatCount == 0.0f) ? 1.0f : fabs(repeatCount);
  f32 result = periodLength()*repeatFactor;
  result = std::max(result, f32(repeatDuration));

  return result;
}

f32 Animation::currentAbsoluteTime(TimeInterval now)
{
  f32 result = 0.0f;
  
  TimeInterval d = now - beginTime - timeOffset;
  f32 td = totalDuration()/speed;
  if(d > 0.0 && (d < td))
  {
    f32 pl = periodLength()/speed;
    f32 inperiod = fmodf(d, pl);
    result = inperiod/pl;
    if(autoreverses)
    {
//      f32 ov = result;
      result = 1.0f-fabsf((result-0.5f)/.5f);
//      DOUT("autoreverse "<<ov<<" -> "<<result);
    }
    result = timingFunction.yAtX(result);
  }
  
  return result;
}

Variant Animation::currentValue(TimeInterval now)
{
  Variant dv = endValue-startValue;
  Variant result = startValue + dv*currentAbsoluteTime(now);
  return result;
}


bool Animation::stopped(TimeInterval now)
{
  bool result = true;
  if(doesRepeatForever())
  {
    result = false;
  }
  else
  {
    f32 td = totalDuration()/speed;
    TimeInterval remainingTime = (beginTime +timeOffset +td) - now;
    if(remainingTime > 0)
    {
      result = false;
    }
    else
    {
      result = true;
    }
  }
  
  return result;
}

bool Animation::doesRepeatForever()
{
  return repeatCount == numeric_limits<float>::infinity();
}

void Animation::repeatForever()
{
  repeatCount = numeric_limits<float>::infinity();
}

}
