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
struct Vec3
{
    union
    {
      struct {float x,y,z;};
      struct {float r,g,b;};
      struct {float v,vt,vn;};
    };



    Vec3();
    Vec3(float inx, float iny, float inz);
    void zero();
};

float len(const Vec3& inOp);
float squlen(const Vec3& inOp);
Vec3& normalise(Vec3& ioOp);
Vec3 operator*(const Vec3& inOp1,  float v);
float operator*(const Vec3& inOp1, const Vec3& inOp2);
Vec3 cross(const Vec3& inOp1, const Vec3& inOp2);
float angle(const Vec3& inOp1, const Vec3& inOp2);
Vec3 operator+(const Vec3& inOp1, const Vec3& inOp2);
void operator +=(Vec3& inOp1, const Vec3& inOp2);
Vec3 operator-(const Vec3& inOp1, const Vec3& inOp2);
void operator -=(Vec3& inOp1, const Vec3& inOp2);
bool operator ==(const Vec3& inOp1, const Vec3& inOp2);
// compares two vectors, taking epsilong into account for rounding errors
bool compare(const Vec3& l, const Vec3& r, float epsilon);

}

