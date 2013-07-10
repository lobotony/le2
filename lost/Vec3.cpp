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

Vec3::Vec3() { zero();}
Vec3::Vec3(float inx, float iny, float inz) : x(inx), y(iny), z(inz) {}
void Vec3::zero() { x = y = z = 0.0f; }

float len(const Vec3& inOp)
{
    return sqrt(inOp.x*inOp.x + inOp.y*inOp.y + inOp.z*inOp.z);
}

float squlen(const Vec3& inOp)
{
    return (inOp.x*inOp.x + inOp.y*inOp.y + inOp.z*inOp.z);
}

Vec3& normalise(Vec3& ioOp)
{
  float l = len(ioOp);
  float x0 = 1.0f/l;

  ioOp.x *= x0;
  ioOp.y *= x0;
  ioOp.z *= x0;
  return ioOp; 
}

Vec3 operator*(const Vec3& inOp1,  float v)
{
    Vec3 result;
    result.x = inOp1.x * v;
    result.y = inOp1.y * v;
    result.z = inOp1.z * v;
    return result;
}



float operator*(const Vec3& inOp1, const Vec3& inOp2)
{
    return (inOp1.x*inOp2.x + inOp1.y*inOp2.y + inOp1.z*inOp2.z);
}

Vec3 cross(const Vec3& inOp1, const Vec3& inOp2)
{
  Vec3 result = 
     Vec3(inOp1.y * inOp2.z - inOp1.z * inOp2.y,
                inOp1.z * inOp2.x - inOp1.x * inOp2.z,
                inOp1.x * inOp2.y - inOp1.y * inOp2.x);
  return result;
}

float angle(const Vec3& inOp1, const Vec3& inOp2)
{
  float result = 0.0f;
  float length = (len(inOp1) * len(inOp2));
  if (length > 0) result = rad2deg(acos((inOp1 * inOp2) / length));
  return result;
}
  
Vec3 operator+(const Vec3& inOp1, const Vec3& inOp2)
{
    Vec3 result;
    result.x = inOp1.x + inOp2.x;
    result.y = inOp1.y + inOp2.y;
    result.z = inOp1.z + inOp2.z;
    return result;
}

void operator +=(Vec3& inOp1, const Vec3& inOp2)
{
    inOp1.x += inOp2.x;
    inOp1.y += inOp2.y;
    inOp1.z += inOp2.z;
}

Vec3 operator-(const Vec3& inOp1, const Vec3& inOp2)
{
    Vec3 result;
    result.x = inOp1.x - inOp2.x;
    result.y = inOp1.y - inOp2.y;
    result.z = inOp1.z - inOp2.z;
    return result;
}

void operator -=(Vec3& inOp1, const Vec3& inOp2)
{
  inOp1.x -= inOp2.x;
  inOp1.y -= inOp2.y;
  inOp1.z -= inOp2.z;
}
  
bool operator ==(const Vec3& inOp1, const Vec3& inOp2)
{
  return (inOp1.x == inOp2.x && inOp1.y == inOp2.y && inOp1.z == inOp2.z);
}
  
// compares two vectors, taking epsilong into account for rounding errors
bool compare(const Vec3& l, const Vec3& r, float epsilon)
{
  if(   compare(l.x, r.x, epsilon)
     && compare(l.y, r.y, epsilon)
     && compare(l.z, r.z, epsilon)
     )
    return true;
  else
    return false;
}

}

