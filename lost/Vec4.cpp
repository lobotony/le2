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

#include "lost/Vec4.h"
#include "lost/lmath.h"
#include <iostream>

namespace lost
{  
Vec4::Vec4()
: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

Vec4::Vec4(float inx, float iny, float inz)
: x(inx), y(iny), z(inz), w(1.0f)
{
}

Vec4::Vec4(float inx, float iny, float inz, float inw)
: x(inx), y(iny), z(inz), w(inw)
{
}

void Vec4::clear()
{
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
  w = 1.0f;    
}

Vec4 operator*(const Vec4& inOp1,  float v)
{
  Vec4 result;
  result.x = inOp1.x * v;
  result.y = inOp1.y * v;
  result.z = inOp1.z * v;
  result.w = inOp1.w * v;
  return result;
}

float operator*(const Vec4& inOp1, const Vec4& inOp2)
{
    return (inOp1.x*inOp2.x + inOp1.y*inOp2.y + inOp1.z*inOp2.z + inOp1.w*inOp2.w);
}

bool operator==(const Vec4& l, const Vec4& r)
{
  if((l.x == r.x) && (l.y == r.y) && (l.z == r.z) && (l.w == r.w))
    return true;
  else
    return false;
}

Vec4 operator+(const Vec4& inOp1, const Vec4& inOp2)
{
  Vec4 result;
  result.x = inOp1.x + inOp2.x;
  result.y = inOp1.y + inOp2.y;
  result.z = inOp1.z + inOp2.z;
  result.w = inOp1.w + inOp2.w;
  return result;
}

Vec4 operator-(const Vec4& inOp1, const Vec4& inOp2)
{
  Vec4 result;
  result.x = inOp1.x - inOp2.x;
  result.y = inOp1.y - inOp2.y;
  result.z = inOp1.z - inOp2.z;
  result.w = inOp1.w - inOp2.w;
  return result;
}

// compares two vectors, taking epsilong into account for rounding errors
bool compare(const Vec4& l, const Vec4& r, float epsilon)
{
  if(   compare(l.x, r.x, epsilon)
     && compare(l.y, r.y, epsilon)
     && compare(l.z, r.z, epsilon)
     && compare(l.w, r.w, epsilon)
     )
    return true;
  else
    return false;
}
}
