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


#ifndef LOST_STRINGSTREAM_H
#define LOST_STRINGSTREAM_H

namespace lost
{

/** Alternative stringstream implementation, replacement for std::strignstream, useful when replacing std::string with EASTL::string
 */
struct StringStream
{
  StringStream();
  virtual ~StringStream();
  void append(const string& d);
  string str();
  void clear();
  
  string _data;
};

//const static string endl;
}

lost::StringStream& operator<<(lost::StringStream& s, float v);
lost::StringStream& operator<<(lost::StringStream& s, double v);
lost::StringStream& operator<<(lost::StringStream& s, int32_t v);
lost::StringStream& operator<<(lost::StringStream& s, uint32_t v);
lost::StringStream& operator<<(lost::StringStream& s, int64_t v);
lost::StringStream& operator<<(lost::StringStream& s, uint64_t v);
lost::StringStream& operator<<(lost::StringStream& s, const char* v);
lost::StringStream& operator<<(lost::StringStream& s, const lost::string& v);
lost::StringStream& operator<<(lost::StringStream& s, const lost::Vec2& v);
lost::StringStream& operator<<(lost::StringStream& s, const lost::Vec3& v);
lost::StringStream& operator<<(lost::StringStream& s, const lost::Vec4& v);
lost::StringStream& operator<<(lost::StringStream& s, const lost::Matrix& v);
lost::StringStream& operator<<(lost::StringStream& s, const lost::Rect& v);
lost::StringStream& operator<<(lost::StringStream& stream, const lost::Path& p);

#endif
