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

#ifndef LOST_RECT_H
#define LOST_RECT_H

#include "lost/Vec2.h"

namespace lost
{
  struct Rect
  {
    Rect(float nx, float ny, float w, float h);
    Rect(float nx, float ny, const Vec2& sz);
    Rect();

    void operator +=( const Vec2& parentPos );
    bool operator ==(const Rect& rect) const;
    bool operator !=(const Rect& rect) const;
    bool operator !();
    Vec2 pos() const;
    Vec2 size() const;
    void clipTo(const Rect& parent);
    float maxX() const;
    float maxY() const;
    float area() const;
    bool fitsInto(const Rect& inTarget) const;    
    bool contains(const Vec2& inPoint) const;
    float left() const;
    float right() const;
    float top() const;
    float bottom() const;

    Vec2 center() const;
    bool intersects(const Rect& r2) const;
    void reset( float inX = 0, float inY = 0, float inWidth = 0, float inHeight = 0 );

    Vec2 bottomLeft() const;
    Vec2 bottomRight() const;
    Vec2 topRight() const;
    Vec2 topLeft() const;
    
    float x;
    float y;
    float width;
    float height;
  };
}

#endif
