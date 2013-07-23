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

#ifndef LOST_UNIFORMBLOCK_H
#define LOST_UNIFORMBLOCK_H

#include "lost/Variant.h"

namespace lost
{

struct UniformBlock
{
  typedef map<string, Variant> VariantMap;
  void setInt(const string& inName, GLint inVal);
  void setFloat(const string& name, float v);
  void setBool(const string& name, bool v);
  void set(const string& name, const Color& v);
  void set(const string& name, const Vec2& v);
  void set(const string& name, const Vec3& v);
  void set(const string& name, const Vec4& v);
  void set(const string& inName, const Matrix& inVal);
  
  static UniformBlockPtr create() { return UniformBlockPtr(new UniformBlock());}
  
  VariantMap variantMap;
};
}

#endif
