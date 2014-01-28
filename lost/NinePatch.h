/*
Copyright (c) 2013 Tony Kostanjsek, Timo Boll

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

#include "lost/Mesh.h"

namespace lost
{

/** A NinePatch is a 2D Quad comprised of a connected 3x3 quad matrix.
 * The corners defined by the left/right/top/bottom parameters are not scaled. Everything else is scaled.
 * The outline of the 3x3 quads will be blown up to match the size of the provided rect.
 * The l/r/t/b measurements should be within the range of the raw pixel data of the texture to
 * achieve meaningful results. Everything else is undefined and will probably look messed up.
 * The specified rect is treated as pixel coordinates in a 2d coordinate system.
 * l,r,t,b are cap sizes in pixels.
 * 0,0 is the bottom left corner.
 */
struct NinePatch : public Mesh
{
  bool flip;
  f32 left;
  f32 right;
  f32 top;
  f32 bottom;
  Vec2 size;
  
  NinePatch();
  
  NinePatch(const TexturePtr& tex,
            const Vec2& size,
            float left,
            float right,
            float top,
            float bottom,
            bool flip); // flips texture coordinates vertically if true
  
  void update(const TexturePtr& tex,
              const Vec2& inSize,
              float left,
              float right,
              float top,
              float bottom);

private:
  void updateVertices();
  void updateTexCoords();
  void updateIndices();
  void init();
};

}

