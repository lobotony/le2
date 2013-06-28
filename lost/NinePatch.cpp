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

#include "lost/NinePatch.h"
#include "lost/Texture.h"
#include "lost/HybridIndexBuffer.h"
#include "lost/HybridVertexBuffer.h"
#include "lost/Rect.h"

namespace lost
{

NinePatch::NinePatch()
{
  flip= false;
  left = 0;
  right = 0;
  top = 0;
  bottom = 0;
  
  init();
}

NinePatch::NinePatch(const TexturePtr& tex,
                      const Vec2& sz,
                      float l,
                      float r,
                      float t,
                      float b,
                      bool f) // flips texture coordinates vertically if true
{
  size = sz;
  flip = f;
  left = l;
  right = r;
  top = t;
  bottom = b;
  
  init();

  this->material->textures.push_back(tex);

  this->update(tex, size, left, right, top, bottom);
}

void NinePatch::init()
{
  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  layout.add(ET_vec2_f32, UT_texcoord0);
  this->resetBuffers(layout, ET_u16);

  indexBuffer->drawMode = GL_TRIANGLES;

  u32 numVertices = 16; // draw it on paper and you'll see it's correct
  uint32_t numQuads = 9; // it's a 3x3 matrix of quads
  uint32_t numTris = numQuads*2; // each quad is drawn with two tris
  u32 numIndices = numTris*3; // currently, each tri is drawn with 3 indices

  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);

//  updateTexCoords();
  updateIndices();

}

void NinePatch::updateIndices()
{
  // now the boring part: create the indices for all the tris
  // this is the pattern:
  // quads from top left to bottom right, scanned horizontally
  // first the top left tri, then the bottom right one
  // top left starts with top left corner of quad,
  // bottom right starts with bottom left corner of the quad
  // all tris counterclockwise        
  this->setIndex(0, 0);
  this->setIndex(1, 4);
  this->setIndex(2, 1);
  this->setIndex(3, 4);
  this->setIndex(4, 5);
  this->setIndex(5, 1);
  this->setIndex(6, 1);
  this->setIndex(7, 5);
  this->setIndex(8, 2);
  this->setIndex(9, 5);
  this->setIndex(10, 6);
  this->setIndex(11, 2);
  this->setIndex(12, 2);
  this->setIndex(13, 6);
  this->setIndex(14, 3);
  this->setIndex(15, 6);
  this->setIndex(16, 7);
  this->setIndex(17, 3);
  this->setIndex(18, 4);
  this->setIndex(19, 8);
  this->setIndex(20, 5);
  this->setIndex(21, 8);
  this->setIndex(22, 9);
  this->setIndex(23, 5);
  this->setIndex(24, 5);
  this->setIndex(25, 9);
  this->setIndex(26, 6);
  this->setIndex(27, 9);
  this->setIndex(28, 10);
  this->setIndex(29, 6);
  this->setIndex(30, 6);
  this->setIndex(31, 10);
  this->setIndex(32, 7);
  this->setIndex(33, 10);
  this->setIndex(34, 11);
  this->setIndex(35, 7);
  this->setIndex(36, 8);
  this->setIndex(37, 12);
  this->setIndex(38, 9);
  this->setIndex(39, 12);
  this->setIndex(40, 13);
  this->setIndex(41, 9);
  this->setIndex(42, 9);
  this->setIndex(43, 13);
  this->setIndex(44, 10);
  this->setIndex(45, 13);
  this->setIndex(46, 14);
  this->setIndex(47, 10);
  this->setIndex(48, 10);
  this->setIndex(49, 14);
  this->setIndex(50, 11);
  this->setIndex(51, 14);
  this->setIndex(52, 15);
  this->setIndex(53, 11);  
}

void NinePatch::updateTexCoords()
{
  TexturePtr tex = material->textures[0];
  
  float tw = (float)std::min(tex->dataWidth, tex->width);
  float th = (float)std::min(tex->dataHeight, tex->height);

  if(flip)
  {
    this->setTexCoord(0, tex->normalisedCoord(Vec2(0,0)));
    this->setTexCoord(1, tex->normalisedCoord(Vec2(left, 0)));
    this->setTexCoord(2, tex->normalisedCoord(Vec2(tw-right, 0)));
    this->setTexCoord(3, tex->normalisedCoord(Vec2(tw, 0)));
    
    this->setTexCoord(4, tex->normalisedCoord(Vec2(0,bottom)));
    this->setTexCoord(5, tex->normalisedCoord(Vec2(left,bottom)));
    this->setTexCoord(6, tex->normalisedCoord(Vec2(tw-right,bottom)));
    this->setTexCoord(7, tex->normalisedCoord(Vec2(tw,bottom)));
    
    this->setTexCoord(8, tex->normalisedCoord(Vec2(0,th-top)));
    this->setTexCoord(9, tex->normalisedCoord(Vec2(left,th-top)));
    this->setTexCoord(10, tex->normalisedCoord(Vec2(tw-right, th-top)));
    this->setTexCoord(11, tex->normalisedCoord(Vec2(tw, th-top)));
    
    this->setTexCoord(12, tex->normalisedCoord(Vec2(0,th)));
    this->setTexCoord(13, tex->normalisedCoord(Vec2(left,th)));
    this->setTexCoord(14, tex->normalisedCoord(Vec2(tw-right,th)));
    this->setTexCoord(15, tex->normalisedCoord(Vec2(tw, th)));
  }
  else
  {
    this->setTexCoord(0, tex->normalisedCoord(Vec2(0,th)));
    this->setTexCoord(1, tex->normalisedCoord(Vec2(left, th)));
    this->setTexCoord(2, tex->normalisedCoord(Vec2(tw-right, th)));
    this->setTexCoord(3, tex->normalisedCoord(Vec2(tw, th)));
    
    this->setTexCoord(4, tex->normalisedCoord(Vec2(0,th-top)));
    this->setTexCoord(5, tex->normalisedCoord(Vec2(left,th-top)));
    this->setTexCoord(6, tex->normalisedCoord(Vec2(tw-right, th-top)));
    this->setTexCoord(7, tex->normalisedCoord(Vec2(tw, th-top)));
    
    this->setTexCoord(8, tex->normalisedCoord(Vec2(0,bottom)));
    this->setTexCoord(9, tex->normalisedCoord(Vec2(left,bottom)));
    this->setTexCoord(10, tex->normalisedCoord(Vec2(tw-right, bottom)));
    this->setTexCoord(11, tex->normalisedCoord(Vec2(tw, bottom)));
    
    this->setTexCoord(12, tex->normalisedCoord(Vec2(0,0)));
    this->setTexCoord(13, tex->normalisedCoord(Vec2(left,0)));
    this->setTexCoord(14, tex->normalisedCoord(Vec2(tw-right,0)));
    this->setTexCoord(15, tex->normalisedCoord(Vec2(tw, 0)));
  }
}

void NinePatch::updateVertices()
{
  // vertices are created in this order:
  // 0  1  2  3
  // 4  5  6  7
  // 8  9  10 11
  // 12 13 14 15
  // 
  // 0,3,12,15 are the corners of the provided rect
  // all others are derived from the l/r/t/b measurement that were provided.
      
  // first, we calculate the vertex coordinates relative to 0,0 since these are the pixel coordinates
  // we need to calculate the texel coordinates from the texture.

  float w = size.x;
  float h = size.y;
  this->setVertex(0, Vec2(0,h));
  this->setVertex(1, Vec2(left, h));
  this->setVertex(2, Vec2(w-right, h));
  this->setVertex(3, Vec2(w,h));
  
  this->setVertex(4, Vec2(0, h-top));
  this->setVertex(5, Vec2(left, h-top));
  this->setVertex(6, Vec2(w-right, h-top));
  this->setVertex(7, Vec2(w, h-top));

  this->setVertex(8, Vec2(0, bottom));
  this->setVertex(9, Vec2(left, bottom));
  this->setVertex(10, Vec2(w-right, bottom));
  this->setVertex(11, Vec2(w, bottom));

  this->setVertex(12, Vec2(0,0));
  this->setVertex(13, Vec2(left, 0));
  this->setVertex(14, Vec2(w-right, 0));
  this->setVertex(15, Vec2(w,0));
}

void NinePatch::update(const TexturePtr& tex,
                      const Vec2& inSize,
                      float l,
                      float r,
                      float t,
                      float b)
{
  size = inSize;
  left = l;
  right = r;
  top = t;
  bottom = b;
  material->setTexture(0, tex);

  updateVertices();
  updateTexCoords();  
}

}

