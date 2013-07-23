//
//  TimingFunction.cpp
//  LostEngine2
//
//  Created by Tony Kostanjsek on 20.07.13.
//  Copyright (c) 2013 Tony Kostanjsek. All rights reserved.
//

#include "TimingFunction.h"

namespace lost
{
  
TimingFunction::TimingFunction(const Vec2& p1, const Vec2& p2)
{
  cp.push_back(Vec2(0,0));
  cp.push_back(p1);
  cp.push_back(p2);
  cp.push_back(Vec2(1,1));
  epsilon = .001;
}

f32 TimingFunction::clamp(f32 v)
{
  v = fminf(v, 1.0f);
  v = fmaxf(v, 0.0f);
  return v;
}

bool TimingFunction::compare(f32 v1, f32 v2)
{
  bool result = (fabs(v1-v2) <= epsilon);
//  DOUT(v1 << " " << (result ? " == " : " != ") << v2);
  return result;
}


Vec2 TimingFunction::pointAt(f32 t)
{
  t = clamp(t);
  if(compare(t,0.0f)) return Vec2(0,0);
  if(compare(t,1.0f)) return Vec2(1,1);
  return bezierInterpolate(t, cp[0], cp[1], cp[2], cp[3]);
}

f32 TimingFunction::yAtX(f32 x)
{
  x = clamp(x);
  if(compare(x,0.0f)) return 0.0f;
  if(compare(x,1.0f)) return 1.0f;
//  DOUT("++++++++++++++ iteration start for "<<x);
  
  f32 upper = 1;
  f32 lower = 0;
  f32 current = .5;
  Vec2 p = pointAt(current);
  u32 numIterations = 0;
  static u32 maxIterations = 10;
  while(!compare(p.x,x) && (numIterations < maxIterations))
  {
    ++numIterations;
    if(x < p.x)
    {
      upper = current;
    }
    else
    {
      lower = current;
    }
    current = (upper-lower)*.5+lower;
    p = pointAt(current);
//    DOUT(lower << " " << current << " " << upper << " = "<<p.x);
  }
//  DOUT(p.y<< " ("<<numIterations<<")");
  return p.y;
}

TimingFunction TimingFunction::linear()
{
  return TimingFunction(Vec2(.5,.5), Vec2(.5,.5));
}

TimingFunction TimingFunction::easeIn()
{
  return TimingFunction(Vec2(.5,0), Vec2(1,.5));
}

TimingFunction TimingFunction::easeOut()
{
  return TimingFunction(Vec2(0,.5), Vec2(.5,1));
}

TimingFunction TimingFunction::easeInOut()
{
  return TimingFunction(Vec2(0,.5), Vec2(1,.5));
}

TimingFunction TimingFunction::defaultFunc()
{
  return easeOut();
}

}
