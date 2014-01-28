#pragma once

namespace lost
{
struct TimingFunction
{
  // fixed: p0(0,0), p3(1,1)
  TimingFunction(const Vec2& p1, const Vec2& p2);

  static TimingFunction linear();
  static TimingFunction easeIn();
  static TimingFunction easeOut();
  static TimingFunction easeInOut();
  static TimingFunction defaultFunc();

  Vec2 pointAt(f32 t); // t = [0,1]
  f32 yAtX(f32 x); // x = [0,1];
  f32 clamp(f32 v);
  bool compare(f32 v1, f32 v2);

  f32 epsilon;
  vector<Vec2> cp;
};
}

