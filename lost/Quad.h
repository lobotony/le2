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

#ifndef LOST_QUAD_H
#define LOST_QUAD_H

#include "lost/Mesh.h"
#include "lost/Data.h"
#include "lost/Texture.h"

namespace lost
{

struct Quad : public Mesh
{
  Quad();
  Quad(const Rect& inRect);
  
  Quad(const DataPtr& data, bool flip=true);
  Quad(const TexturePtr& tex, bool flip=true);
  Quad(const vector<Rect>& rects);
  
  Quad(const vector<Rect>& rects,
       TexturePtr tex,
       const vector<Rect>& pixelCoords,
       bool flip);
  
  void init(const vector<Rect>& rects,
            TexturePtr tex,
            const vector<Rect>& pixelCoords,
            bool flip);
  
  static QuadPtr create() { return QuadPtr(new Quad()); }
  static QuadPtr create(const Rect& inRect) { return QuadPtr(new Quad(inRect)); }
  static QuadPtr create(const vector<Rect>& rects) { return QuadPtr(new Quad(rects)); }
  static QuadPtr create(const DataPtr& data, bool flip=true) { return QuadPtr(new Quad(data, flip)); }
  static QuadPtr create(const TexturePtr& tex, bool flip=true) { return QuadPtr(new Quad(tex, flip)); }
  
  virtual ~Quad();
  
  void createIndices(uint32_t quadNum);
  void createVertices(uint32_t quadNum, const Rect& inRect);
  void updateSize(const Vec2& size);

  void createTexCoords(uint32_t quadNum,
                       bool flip=true);

  void createTexCoords(uint32_t quadNum,
                       uint32_t texNum,
                       bool flip=true);
  
  void createTexCoords(uint32_t quadNum,
                       uint32_t texNum,
                       const Rect& pixelRect,
                       bool flip=true);         
};
}
#endif
