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
#include "lost/Vec3.h"
#include "lost/Vec2.h"
#include "lost/Rect.h"
#include "lost/Matrix.h"
#include "lost/lmath.h"
#include <iostream>
#include "lost/Log.h"

namespace lost
{

void Matrix::zero()
{
  for(unsigned long i=0; i<numvals; ++i)
  {
    m[i] = 0.0f;
  }
}

void Matrix::initIdentity()
{
  zero();
  m[0] = 1.0f;
  m[5] = 1.0f;
  m[10] = 1.0f;
  m[15] = 1.0f;
}

void Matrix::initTranslation(const Vec3& t)
{
  initIdentity();
  m[12] = t.x;
  m[13] = t.y;
  m[14] = t.z;
}

void Matrix::initScaling(const Vec3& s)
{
  initIdentity();
  m[0] = s.x;
  m[5] = s.y;
  m[10] = s.z;
}

void Matrix::initRotateX(float angleDeg)
{
  initIdentity();
  float ar = deg2rad(angleDeg);
  m[5] = cos(ar);m[9] = sin(ar);
  m[6] = -1.0f*sin(ar);m[10] = cos(ar);
}

void Matrix::initRotateY(float angleDeg)
{
  initIdentity();
  float ar = deg2rad(angleDeg);
  m[0]=cos(ar); m[8]=-1.0f*sin(ar);
  m[2]=sin(ar); m[10]=cos(ar);
}

void Matrix::initRotateZ(float angleDeg)
{
  initIdentity();
  float ar = deg2rad(angleDeg);
  m[0]=cos(ar); m[4]=sin(ar);
  m[1]=-1.0f*sin(ar); m[5]=cos(ar);
}

void Matrix::initOrtho(const Rect& rect, const Vec2& nearAndFar)
{
  zero();
  m[0] = 2 / (rect.width - rect.x);
  m[5] = 2 / (rect.height - rect.y);
  m[10] = -2 / (nearAndFar.max - nearAndFar.min);
  m[12] = -((rect.width + rect.x) / (rect.width - rect.x));
  m[13] = -((rect.height + rect.y) / (rect.height - rect.y));
  m[14] = -((nearAndFar.max + nearAndFar.min) / (nearAndFar.max - nearAndFar.min));
  m[15] = 1.0;
}

void Matrix::initPerspective(const float& fovy, const float& aspect, const Vec2& nearAndFar)
{
  float radFovY = deg2rad(fovy / 2.0f);
  float f = cos(radFovY) / sin(radFovY);
  float deltaZ = nearAndFar.max - nearAndFar.min;
  
  float m0 = f / aspect;
  float m5 = f;
  float m10 = - (nearAndFar.max + nearAndFar.min) / deltaZ;
  float m11 = -1;
  float m14 = (-2 * nearAndFar.min * nearAndFar.max) / deltaZ;
  
  zero();
  m[0] = m0;
  m[5] = m5;
  m[10] = m10;
  m[11] = m11;
  m[14] = m14;
}

void Matrix::initLookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
{
  Vec3 upNormal = up;
  Vec3 f = center - eye;
  normalise(f);
  normalise(upNormal);
  Vec3 s = cross(f, upNormal);
  Vec3 u = cross(s, f);

  zero();

  m[0] = s.x;
  m[1] = u.x;
  m[2] = -1.0f*f.x;

  m[4] = s.y;
  m[5] = u.y;
  m[6] = -1.0f*f.y;

  m[8] = s.z;
  m[9] = u.z;
  m[10] = -1.0f*f.z;

  m[15] = 1;

  Matrix translation;
  translation.initTranslation(eye * -1.0f);
  *this = *this * translation;
}
void Matrix::transpose()
{
  std::swap(m[1], m[4]);
  std::swap(m[2], m[8]);
  std::swap(m[6], m[9]);std::swap(m[3], m[12]);
  std::swap(m[7], m[13]);
  std::swap(m[11], m[14]);
}
Vec4 Matrix::row(long num) const
{
  assert((num >= 0) && (num <=3));
  switch(num)
  {
    case 0:return Vec4(m[0], m[4], m[8], m[12]);
    case 1:return Vec4(m[1], m[5], m[9], m[13]);
    case 2:return Vec4(m[2], m[6], m[10], m[14]);
    case 3:return Vec4(m[3], m[7], m[11], m[15]);
    default: ASSERT(false, "impossible");return Vec4();
  }
}
void Matrix::row(long num, const Vec4& row)
{
  assert((num >= 0) && (num <=3));
  switch(num)
  {
    case 0: m[0] = row.x; m[4] = row.y; m[8] = row.z; m[12] = row.w; break;
    case 1: m[1] = row.x; m[5] = row.y; m[9] = row.z; m[13] = row.w; break;
    case 2: m[2] = row.x; m[6] = row.y; m[10] = row.z; m[14] = row.w; break;
    case 3: m[3] = row.x; m[7] = row.y; m[11] = row.z; m[15] = row.w; break;
    default: ASSERT(false, "impossible");
  }
}
Vec4 Matrix::col(long num) const
{
  assert((num >= 0) && (num <=3));
  switch(num)
  {
    case 0:return Vec4(m[0], m[1], m[2], m[3]);
    case 1:return Vec4(m[4], m[5], m[6], m[7]);
    case 2:return Vec4(m[8], m[9], m[10], m[11]);
    case 3:return Vec4(m[12], m[13], m[14], m[15]);
    default: ASSERT(false, "impossible");return Vec4();
  }
}
Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
  Matrix result;

  Vec4 tmp = rhs.col(0);
  result.m[0] = lhs.row(0) * tmp;
  result.m[1] = lhs.row(1) * tmp;
  result.m[2] = lhs.row(2) * tmp;
  result.m[3] = lhs.row(3) * tmp;

  tmp = rhs.col(1);
  result.m[4] = lhs.row(0) * tmp;
  result.m[5] = lhs.row(1) * tmp;
  result.m[6] = lhs.row(2) * tmp;
  result.m[7] = lhs.row(3) * tmp;

  tmp = rhs.col(2);
  result.m[8] = lhs.row(0) * tmp;
  result.m[9] = lhs.row(1) * tmp;
  result.m[10] = lhs.row(2) * tmp;
  result.m[11] = lhs.row(3) * tmp;

  tmp = rhs.col(3);
  result.m[12] = lhs.row(0) * tmp;
  result.m[13] = lhs.row(1) * tmp;
  result.m[14] = lhs.row(2) * tmp;
  result.m[15] = lhs.row(3) * tmp;

  return result;
}
Vec3 operator*(const Matrix& lhs, const Vec3& rhs)
{
  Vec4 result;
  Vec4 temp;

  temp.x = rhs.x;
  temp.y = rhs.y;
  temp.z = rhs.z;
  temp.w = 1.0f;

  result.x = lhs.row(0) * temp;
  result.y = lhs.row(1) * temp;
  result.z = lhs.row(2) * temp;
  result.w = lhs.row(3) * temp;

  return Vec3(result.x, result.y, result.z);
}

Vec4 operator*(const Matrix& lhs, const Vec4& rhs)
{
  Vec4 result;

  result.x = lhs.row(0) * rhs;
  result.y = lhs.row(1) * rhs;
  result.z = lhs.row(2) * rhs;
  result.w = lhs.row(3) * rhs;

  return result;
}

bool operator==(const Matrix& lhs, const Matrix& rhs)
{
  bool result = true;

  for(unsigned long i=0; i<Matrix::numvals; ++i)
  {
    if(lhs.m[i] != rhs.m[i])
    {
      result = false; break;
    }
  }

  return result;
}

bool operator!=(const Matrix& lhs, const Matrix& rhs)
{
  return !(lhs == rhs);
}

}

