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

#ifndef LOST_PATH_H
#define LOST_PATH_H

namespace lost 
{
  struct Path
  {
    Path();
    Path(const char* p);
    Path(const string& sp);
    virtual ~Path();

    void operator /= (const Path& other);
    void operator /= (const char* other);
    void operator = (const char* other);
    bool operator ==(const char* other);
    bool operator !=(const char* other);

    Path directory();
    Path file();

    Path relativeTo(const Path& other);

    lost::string string() const;
    lost::string native() const;

    lost::string path;
  };

  Path operator / (const Path& left, const Path& right);
  Path operator + (const Path& left, const char* right);

  bool exists(const Path& path);
  bool create_directories(const Path& path); 
}

#endif
