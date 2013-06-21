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

#ifndef LOST_COLOR_H
#define LOST_COLOR_H

namespace lost
{
  struct Color
  {
    Color();
    Color(float newr, float newg, float newb);
    Color(float newr, float newg, float newb, float newa);

     float r() const;
     float g() const;
     float b() const;
     float a() const;

     void r(float v);
     void g(float v);
     void b(float v);
     void a(float v);

    // accessors for rgba values as usingned 8 bit integers
     uint8_t ru8() const;
     uint8_t gu8() const;
     uint8_t bu8() const;
     uint8_t au8() const;

     void ru8(uint8_t v);
     void gu8(uint8_t v);
     void bu8(uint8_t v);
     void au8(uint8_t v);
    
    float fv[4];

    Color premultiplied(); // creates a new color object from the current one with color values premultiplied with alpha
  };


  lost::Color operator*(const Color& l, const Color& r);
  lost::Color operator*(const Color& l, float);
  bool operator ==(const Color& l, const Color& r);
  bool operator !=(const Color& l, const Color& r);
  void operator+=(Color& l, const Color& r);
  void operator-=(Color& l, const Color& r);
  void operator/=(Color& l, float);

  extern const lost::Color clearColor      ;
  extern const lost::Color whiteColor      ;
  extern const lost::Color blackColor      ;
  extern const lost::Color redColor        ;
  extern const lost::Color greenColor      ;
  extern const lost::Color blueColor       ;
  extern const lost::Color yellowColor     ;
  extern const lost::Color grayColor       ;
}

#endif
