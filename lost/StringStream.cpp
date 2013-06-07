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

#include "lost/StringStream.h"
#include "lost/types.h"

namespace lost
{

StringStream::StringStream()
{
}

StringStream::~StringStream()
{
}

void StringStream::append(const string& d)
{
  _data.append(d);
}

string StringStream::str()
{
  return _data;
}

void StringStream::clear()
{
  _data="";
}

static const string endl = "\n";
}

lost::StringStream& operator<<(lost::StringStream& s, float v)
{
  char buf[256];
  sprintf(buf, "%f",v);
  s.append(buf);
  return s;
  
}

lost::StringStream& operator<<(lost::StringStream& s, double v)
{
  char buf[256];
  sprintf(buf, "%lf",v);
  s.append(buf);
  return s;  
}

lost::StringStream& operator<<(lost::StringStream& s, int32_t v)
{
  char buf[32];
  sprintf(buf, "%d",v);
  s.append(buf);
  return s;
}

lost::StringStream& operator<<(lost::StringStream& s, uint32_t v)
{
  char buf[32];
  sprintf(buf, "%u",v);
  s.append(buf);
  return s;
}

lost::StringStream& operator<<(lost::StringStream& s, int64_t v)
{
  char buf[32];
  sprintf(buf, "%lld",v);
  s.append(buf);
  return s;
}

lost::StringStream& operator<<(lost::StringStream& s, uint64_t v)
{
  char buf[32];
  sprintf(buf, "%llu",v);
  s.append(buf);
  return s;
}

lost::StringStream& operator<<(lost::StringStream& s, const char* v)
{
  s.append(v);
  return s;
}

lost::StringStream& operator<<(lost::StringStream& s, const lost::string& v)
{
  s.append(v);
  return s;
}

lost::StringStream& operator<<(lost::StringStream& s, const lost::Vec2& v)
{
  s << "( "<<v.x<<" / "<<v.y<<" )";
  return s;
}

lost::StringStream& operator<<(lost::StringStream& s, const lost::Vec3& v)
{
  s << "( "<<v.x<<" / "<<v.y<<" / "<<v.z<<" )";
  return s;
}

lost::StringStream& operator<<(lost::StringStream& s, const lost::Vec4& v)
{
  s << "( "<<v.x<<" / "<<v.y<<" / "<<v.z<<" / "<<v.w<<" )";
  return s;
}

lost::StringStream& operator<<(lost::StringStream& s, const lost::Matrix& v)
{
  for(lost::u32 i=0; i<lost::Matrix::numvals; ++i)
  {
    s << "\n" ;
    for(lost::u32 j=0; j<4; ++j)
    {
      s << v.col(j) << "\n";
    }
  }
  return s;
}



