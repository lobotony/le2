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

#include "lost/Quad.h"
#include "lost/HybridBuffer.h"

namespace lost
{

Quad::Quad()
{
  indexBuffer->drawMode = GL_TRIANGLES;
}

Quad::Quad(const Rect& inRect)
{
  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  layout.add(ET_vec3_f32, UT_normal);
  layout.add(ET_vec2_f32, UT_texcoord0);
  this->resetBuffers(layout, ET_u16);
  
  indexBuffer->drawMode = GL_TRIANGLES;
  uint32_t numQuads = 1;
  uint32_t numVertices = numQuads*4;
  uint32_t numIndices = numQuads*6;
  
  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);
  
  createIndices(0);
  createVertices(0,inRect);
}

Quad::Quad(const DataPtr& data, bool flip)
{
  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  layout.add(ET_vec3_f32, UT_normal);
  layout.add(ET_vec2_f32, UT_texcoord0);
  this->resetBuffers(layout, ET_u16);

  indexBuffer->drawMode = GL_TRIANGLES;
  TexturePtr tex(new Texture(data));
  Rect rect(0, 0, (float)tex->dataWidth, (float)tex->dataHeight);
  this->material->textures.push_back(tex);
  uint32_t numQuads = 1;
  uint32_t numVertices = numQuads*4;
  uint32_t numIndices = numQuads*6;
  
  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);
  
  createIndices(0);
  createVertices(0, rect);
  createTexCoords(0,flip);
}

Quad::Quad(const TexturePtr& tex, bool flip)
{
  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  layout.add(ET_vec3_f32, UT_normal);
  layout.add(ET_vec2_f32, UT_texcoord0);
  this->resetBuffers(layout, ET_u16);

  indexBuffer->drawMode = GL_TRIANGLES;
  Rect rect(0, 0, (float)tex->dataWidth, (float)tex->dataHeight);
  this->material->textures.push_back(tex);
  uint32_t numQuads = 1;
  uint32_t numVertices = numQuads*4;
  uint32_t numIndices = numQuads*6;

  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);

  createIndices(0);
  createVertices(0, rect);
  createTexCoords(0,flip);    
}

Quad::Quad(const vector<Rect>& rects)
{
  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  layout.add(ET_vec3_f32, UT_normal);
  layout.add(ET_vec2_f32, UT_texcoord0);  
  this->resetBuffers(layout, ET_u16);

  indexBuffer->drawMode = GL_TRIANGLES;
  size_t numQuads = rects.size();
  size_t numVertices = numQuads*4;
  size_t numIndices = numQuads*6;

  this->vertexBuffer->reset((uint32_t)numVertices);
  this->indexBuffer->reset((uint32_t)numIndices);

  for(uint32_t i=0; i<numQuads; ++i)
  {
    createIndices(i);
    createVertices(i,rects[i]);
  }
}

Quad::Quad(const vector<Rect>& rects,
     TexturePtr tex,
     const vector<Rect>& pixelCoords,
     bool flip)
{
  this->init(rects, tex, pixelCoords, flip);
}

void Quad::init(const vector<Rect>& rects,
          TexturePtr tex,
          const vector<Rect>& pixelCoords,
          bool flip)
{
  ASSERT(rects.size() == pixelCoords.size(), "number of rects and pixelCoords must match");
  
  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  layout.add(ET_vec3_f32, UT_normal);
  layout.add(ET_vec2_f32, UT_texcoord0);
  this->resetBuffers(layout, ET_u16);
  
  indexBuffer->drawMode = GL_TRIANGLES;
  this->material->textures.clear();
  this->material->textures.push_back(tex);
  size_t numQuads = rects.size();
  size_t numVertices = numQuads*4;
  size_t numIndices = numQuads*6;
  
  this->vertexBuffer->reset((uint32_t)numVertices);
  this->indexBuffer->reset((uint32_t)numIndices);
  
  for(uint32_t i=0; i<numQuads; ++i)
  {
    createIndices(i);
    createVertices(i, rects[i]);
    createTexCoords(i, 0, pixelCoords[i], flip);
  }
}

Quad::~Quad()
{
}

void Quad::createIndices(uint32_t quadNum)
{ 
  const uint32_t numIndicesPerQuad = 6;
  uint32_t indexOffset = quadNum*numIndicesPerQuad;
  uint32_t vertexOffset = quadNum*4;
  this->setIndex(indexOffset+0, vertexOffset + 0);
  this->setIndex(indexOffset+1, vertexOffset + 1);
  this->setIndex(indexOffset+2, vertexOffset + 2);
  this->setIndex(indexOffset+3, vertexOffset + 2);
  this->setIndex(indexOffset+4, vertexOffset + 3);
  this->setIndex(indexOffset+5, vertexOffset + 0);    
}

void Quad::createVertices(uint32_t quadNum, const Rect& inRect)
{
  uint32_t verticesPerQuad = 4;
  uint32_t offset = quadNum*verticesPerQuad;

  this->setVertex(offset+0, Vec2(inRect.x,inRect.y));
  this->setVertex(offset+1, Vec2(inRect.x+inRect.width,inRect.y));
  this->setVertex(offset+2, Vec2(inRect.x+inRect.width,inRect.y+inRect.height));
  this->setVertex(offset+3, Vec2(inRect.x,inRect.y+inRect.height));

  // calculate normals
  Vec3 v1(inRect.x,inRect.y,0);
  Vec3 v2(inRect.x+inRect.width,inRect.y,0);
  Vec3 v3(inRect.x+inRect.width,inRect.y+inRect.height,0);
  Vec3 v4(inRect.x,inRect.y+inRect.height,0);
  Vec3 normal1 = cross(v2-v1, v3-v2);
  normalise(normal1);
  Vec3 normal2 = cross(v4-v3, v1-v4);
  normalise(normal2);
  this->vertexBuffer->set(offset+0, UT_normal, normal1+normal2);
  this->vertexBuffer->set(offset+1, UT_normal, normal1);
  this->vertexBuffer->set(offset+2, UT_normal, normal1+normal2);
  this->vertexBuffer->set(offset+3, UT_normal, normal1);
}
            
void Quad::updateSize(const Vec2& size)
{
  Rect rect(0, 0, size.width, size.height);
  createVertices(0, rect);
}

void Quad::createTexCoords(uint32_t quadNum,
                     bool flip)
{
  createTexCoords(quadNum, 0, flip);
}

void Quad::createTexCoords(uint32_t quadNum,
                     uint32_t texNum,
                     bool flip)
{
  TexturePtr tex = this->material->textures[texNum];
  Rect pixelCoords(0, 0, (float)tex->dataWidth, (float)tex->dataHeight);
  createTexCoords(quadNum, texNum, pixelCoords, flip);
}

void Quad::createTexCoords(uint32_t quadNum,
                     uint32_t texNum,
                     const Rect& pixelRect,
                     bool flip)
{
  TexturePtr tex = this->material->textures[texNum];

  Vec2 bl = tex->normalisedCoord(pixelRect.bottomLeft());
  Vec2 tr = tex->normalisedCoord(Vec2(pixelRect.x+pixelRect.width, pixelRect.y+pixelRect.height));
  
  const uint32_t texCoordsPerQuad = 4; 
  uint32_t offset = texCoordsPerQuad*quadNum;
  
  if(flip)
  {
    this->setTexCoord(offset+0, Vec2(bl.x,tr.y));
    this->setTexCoord(offset+1, Vec2(tr.x,tr.y));
    this->setTexCoord(offset+2, Vec2(tr.x,bl.y));
    this->setTexCoord(offset+3, Vec2(bl.x,bl.y));
  }
  else
  {
    this->setTexCoord(offset+0, Vec2(bl.x,bl.y));
    this->setTexCoord(offset+1, Vec2(tr.x,bl.y));
    this->setTexCoord(offset+2, Vec2(tr.x,tr.y));
    this->setTexCoord(offset+3, Vec2(bl.x,tr.y));
  }
}

}
