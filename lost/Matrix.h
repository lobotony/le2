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
struct Matrix;
Matrix operator*(const Matrix& lhs, const Matrix& rhs);
Vec3 operator*(const Matrix& lhs, const Vec3& rhs);
Vec4 operator*(const Matrix& lhs, const Vec4& rhs);
bool operator==(const Matrix& lhs, const Matrix& rhs);
bool operator!=(const Matrix& lhs, const Matrix& rhs);

/* OpenGL compatible 4x4 matrix in column major order
 * From the blue book:
 *
 * m[0] m[4] m[8]  m[12]
 * m[1] m[5] m[9]  m[13]
 * m[2] m[6] m[10] m[14]
 * m[3] m[7] m[11] m[15]
 */
struct Matrix
{
  static const u32 numvals = 16; // named constant to avoid magic number
  float m[numvals]; // holds all matrix values as required by OpenGL

  void zero(); // zero all elements      
  void initIdentity(); // create identity matrix      
  void initTranslation(const Vec3& t); // initialises the matrix to be a translation matrix with the given values

  // initialises the matrix to be a scaling matrix with the given values
  // each component of the vector denotes a scaling factor along the corresponding axis
  void initScaling(const Vec3& s);
  void initRotateX(f32 angleDeg);
  void initRotateY(f32 angleDeg);
  void initRotateZ(f32 angleDeg);
  void initOrtho(const Rect& rect, const Vec2& nearAndFar);
  void initPerspective(f32 fovy, f32 aspect, const Vec2& nearAndFar);
  void initLookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
  void transpose();


  static Matrix rotate(const Vec3& inAngles)
  {
    Matrix m;
    m.initRotateX(inAngles.x);
    m.initRotateY(inAngles.y);
    m.initRotateZ(inAngles.z);
    return m;
  }
  
  static Matrix rotateX(f32 angle)
  {
    Matrix m;
    m.initRotateX(angle);
    return m;
  }

  static Matrix rotateY(f32 angle)
  {
    Matrix m;
    m.initRotateY(angle);
    return m;
  }

  static Matrix rotateZ(f32 angle)
  {
    Matrix m;
    m.initRotateZ(angle);
    return m;
  }
  
  static Matrix translate(const Vec3& inTranslation)
  {
    Matrix m;
    m.initTranslation(inTranslation);
    return m;
  }
  static Matrix translate(f32 x, f32 y) { return translate(Vec3(x,y,0)); }
  
  
  static Matrix ortho(const Rect& rect, const Vec2& nearAndFar)
  {
    Matrix m;
    m.initOrtho(rect, nearAndFar);
    return m;
  }
  
  static Matrix perspective(f32 fovy,
                            f32 aspect,
                            const Vec2& nearAndFar)
  {
    Matrix m;
    m.initPerspective(fovy, aspect, nearAndFar);
    return m;
  }
  
  static Matrix lookAt(const Vec3& eye,
                       const Vec3& center,
                       const Vec3& up)
  {
    Matrix m;
    m.initLookAt(eye, center, up);
    return m;
  }
  
  static Matrix scale(const Vec3& f)
  {
    Matrix m;
    m.initScaling(f);
    return m;
  }
  static Matrix scale(f32 x, f32 y) { return scale(Vec3(x,y,1)); }
  
  static Matrix identity()
  {
    Matrix m;
    m.initIdentity();
    return m;
  }
  
  Vec4 row(u32 num) const; // return the specified row as vec4
  void row(u32 num, const Vec4& row); // set the specified row as vec4
  Vec4 col(u32 num) const; // return the specified column as vec4
};

Matrix operator*(const Matrix& lhs, const Matrix& rhs);
Matrix operator*(const Matrix& lhs, f32 rhs);
Vec3 operator*(const Matrix& lhs, const Vec3& rhs);
Vec4 operator*(const Matrix& lhs, const Vec4& rhs);

bool operator==(const Matrix& lhs, const Matrix& rhs);
bool operator!=(const Matrix& lhs, const Matrix& rhs);



}

