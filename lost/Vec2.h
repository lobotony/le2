/*
Copyright (c) 2012 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

namespace lost
{
  struct Vec2
  {
      union
      {
        struct {float x,y;};
        struct {float width,height;};
        struct {float min,max;};
      };

      Vec2(float a, float b);
      Vec2();
      void zero();
  };

  float len(const Vec2& inOp);
  float squlen(const Vec2& inOp);
  Vec2& normalise(Vec2& ioOp);
  Vec2 perpendicular(Vec2& v);
  Vec2 operator*(const Vec2& inOp1,  float v);
  Vec2 operator*(float v, const Vec2& inOp1);
  float operator*(const Vec2& inOp1, const Vec2& inOp2);
  Vec2 operator+(const Vec2& inOp1, const Vec2& inOp2);
  void operator +=(Vec2& inOp1, const Vec2& inOp2);
  Vec2 operator-(const Vec2& inOp1, const Vec2& inOp2);
  bool operator==(const Vec2& inOp1, const Vec2& inOp2);
  bool operator!=(const Vec2& inOp1, const Vec2& inOp2);
  float angle(const Vec2& inOp1, const Vec2& inOp2);

  // compares two vectors, taking epsilon into account for rounding errors
  bool compare(const Vec2& l, const Vec2& r, float epsilon);
}

