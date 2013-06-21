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

#include "lost/Color.h"
#include <assert.h>

namespace lost
{
  const Color clearColor      (0.0f, 0.0f, 0.0f, 0.0f);
  const Color whiteColor      (1.0f, 1.0f, 1.0f, 1.0f);
  const Color blackColor      (0.0f, 0.0f, 0.0f, 1.0f);
  const Color redColor        (1.0f, 0.0f, 0.0f, 1.0f);
  const Color greenColor      (0.0f, 1.0f, 0.0f, 1.0f);
  const Color blueColor       (0.0f, 0.0f, 1.0f, 1.0f);
  const Color yellowColor     (1.0f, 1.0f, 0.0f, 1.0f);
  const Color grayColor       (0.5f, 0.5f, 0.5f, 1.0f);


    Color::Color() {r(1.0f); g(1.0f); b(1.0f); a(1.0f); }

    Color::Color(float newr, float newg, float newb) {r(newr); g(newg); b(newb); a(1.0f); }
    Color::Color(float newr, float newg, float newb, float newa) {r(newr); g(newg); b(newb); a(newa); }

     float Color::r() const { return fv[0]; }
     float Color::g() const { return fv[1]; }
     float Color::b() const { return fv[2]; }
     float Color::a() const { return fv[3]; }

     void Color::r(float v) { fv[0] = v; }
     void Color::g(float v) { fv[1] = v; }
     void Color::b(float v) { fv[2] = v; }
     void Color::a(float v) { fv[3] = v; }

    // accessors for rgba values as usingned 8 bit integers
     uint8_t Color::ru8() const { return (uint8_t)(fv[0]*255); };
     uint8_t Color::gu8() const { return (uint8_t)(fv[1]*255); };
     uint8_t Color::bu8() const { return (uint8_t)(fv[2]*255); };
     uint8_t Color::au8() const { return (uint8_t)(fv[3]*255); };

     void Color::ru8(uint8_t v) { fv[0] = float(v)/255.0f; }
     void Color::gu8(uint8_t v) { fv[1] = float(v)/255.0f; }
     void Color::bu8(uint8_t v) { fv[2] = float(v)/255.0f; }
     void Color::au8(uint8_t v) { fv[3] = float(v)/255.0f; }

  Color Color::premultiplied()
  {
    return Color(fv[0]*fv[3], fv[1]*fv[3], fv[2]*fv[3], fv[3]);
  }


  Color operator*(const Color& l, const Color& r)
  {
    Color result;

    result.fv[0] = l.fv[0] * r.fv[0];
    result.fv[1] = l.fv[1] * r.fv[1];
    result.fv[2] = l.fv[2] * r.fv[2];
    result.fv[3] = l.fv[3] * r.fv[3];

    return result;
  }

  Color operator*(const Color& l, float f)
  {
    Color result;

    result.fv[0] = l.fv[0] * f;
    result.fv[1] = l.fv[1] * f;
    result.fv[2] = l.fv[2] * f;
    result.fv[3] = l.fv[3] * f;

    return result;
  }


  bool operator ==(const Color& l, const Color& r)
  {
    return (l.fv[0] == r.fv[0]) && (l.fv[1] == r.fv[1]) && (l.fv[2] == r.fv[2]) && (l.fv[3] == r.fv[3]);
  }
  
  bool operator !=(const Color& l, const Color& r)
  {
    return !(l==r);
  }
  
  void operator+=(Color& l, const Color& r)
  {
    l.fv[0] = std::min( 1.0f, l.fv[0] + r.fv[0] );
    l.fv[1] = std::min( 1.0f, l.fv[1] + r.fv[1] );
    l.fv[2] = std::min( 1.0f, l.fv[2] + r.fv[2] );
    l.fv[3] = std::min( 1.0f, l.fv[3] + r.fv[3] );
  }

  void operator-=(Color& l, const Color& r)
  {
    l.fv[0] = std::max( 0.0f, l.fv[0] - r.fv[0] );
    l.fv[1] = std::max( 0.0f, l.fv[1] - r.fv[1] );
    l.fv[2] = std::max( 0.0f, l.fv[2] - r.fv[2] );
    l.fv[3] = std::max( 0.0f, l.fv[3] - r.fv[3] );
  }

  void operator/=(Color& l, float f)
  {
    assert(f != 0.0f);

    l.fv[0] = l.fv[0] / f;
    l.fv[1] = l.fv[1] / f;
    l.fv[2] = l.fv[2] / f;
    l.fv[3] = l.fv[3] / f;
  }
}
