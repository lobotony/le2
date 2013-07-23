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

namespace lost
{

  Vec2::Vec2(float a, float b) : x(a), y(b) {}
  Vec2::Vec2() { zero();}
  void Vec2::zero() { x = y = 0.0f; }

  float len(const Vec2& inOp)
  {
    return sqrt(inOp.x*inOp.x + inOp.y*inOp.y);
  }

  float squlen(const Vec2& inOp)
  {
    return (inOp.x*inOp.x + inOp.y*inOp.y);
  }

  Vec2& normalise(Vec2& ioOp)
  {
    float l = len(ioOp);
    float x0 = 1.0f/l;

    ioOp.x *= x0;
    ioOp.y *= x0;
    return ioOp;
  }

  Vec2 perpendicular(Vec2& v)
  {
    Vec2 result;

    result.x = -v.y;
    result.y = v.x;

    return result;
  }

  Vec2 operator*(const Vec2& inOp1,  float v)
  {
    Vec2 result;
    result.x = inOp1.x * v;
    result.y = inOp1.y * v;
    return result;
  }

  Vec2 operator*(float v, const Vec2& inOp1)
  {
    return inOp1*v;
  }

  float operator*(const Vec2& inOp1, const Vec2& inOp2)
  {
    return (inOp1.x*inOp2.x + inOp1.y*inOp2.y);
  }

  Vec2 operator+(const Vec2& inOp1, const Vec2& inOp2)
  {
    Vec2 result;
    result.x = inOp1.x + inOp2.x;
    result.y = inOp1.y + inOp2.y;
    return result;
  }

  void operator +=(Vec2& inOp1, const Vec2& inOp2)
  {
    inOp1.x += inOp2.x;
    inOp1.y += inOp2.y;
  }

  Vec2 operator-(const Vec2& inOp1, const Vec2& inOp2)
  {
    Vec2 result;
    result.x = inOp1.x - inOp2.x;
    result.y = inOp1.y - inOp2.y;
    return result;
  }

  bool operator==(const Vec2& inOp1, const Vec2& inOp2)
  {
    return (inOp1.x == inOp2.x && inOp1.y == inOp2.y);
  }

  bool operator!=(const Vec2& inOp1, const Vec2& inOp2)
  {
    return (inOp1.x != inOp2.x && inOp1.y != inOp2.y);
  }

  float angle(const Vec2& inOp1, const Vec2& inOp2)
  {
    float result = 0.0f;
    float length = (len(inOp1) * len(inOp2));
    if (length > 0) result = rad2deg(acos((inOp1 * inOp2) / length));
    return result;
  }

  // compares two vectors, taking epsilon into account for rounding errors
  bool compare(const Vec2& l, const Vec2& r, float epsilon)
  {
    if(   compare(l.x, r.x, epsilon)
       && compare(l.y, r.y, epsilon)
       )
      return true;
    else
      return false;
  }
}
