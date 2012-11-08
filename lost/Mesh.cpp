/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

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

#include "lost/Mesh.h"
#include "lost/HostBuffer.h"
#include "lost/HybridVertexBuffer.h"
#include "lost/HybridIndexBuffer.h"

namespace lost
{

Mesh::Mesh()
{
  BufferLayout layout;
  layout.add(ET_vec3_f32, UT_position);
  layout.add(ET_vec2_f32, UT_texcoord0);
  init(layout, ET_u16);
}

Mesh::Mesh(const BufferLayout& vertexLayout, ElementType indexType)
{
  init(vertexLayout, indexType);
}

Mesh::~Mesh() 
{
//  DOUT("destroying mesh");
}

void Mesh::init(const BufferLayout& vertexLayout, ElementType indexType)
{
//  DOUT("creating mesh");
  material = Material::create();
  transform.initIdentity();
  resetBuffers(vertexLayout, indexType);
}

void Mesh::resetBuffers(const BufferLayout& vertexLayout, ElementType indexType)
{
  vertexBuffer.reset(new HybridVertexBuffer(vertexLayout));
  indexBuffer.reset(new HybridIndexBuffer(indexType));
}

void Mesh::resetSize(uint32_t numVertices, uint32_t numIndices)
{
  vertexBuffer->reset(numVertices);
  indexBuffer->reset(numIndices);
}

HybridBufferPtr Mesh::bufferFromUsageType(UsageType ut)
{
  if(ut == UT_index)
    return indexBuffer;
  else
    return vertexBuffer;
}

void Mesh::setIndex(uint32_t idx, uint32_t val)
{
  indexBuffer->set(idx, UT_index, val);
}

void Mesh::setVertex(uint32_t idx, const Vec2& val)
{
  vertexBuffer->set(idx, UT_position, val);
}

void Mesh::setTexCoord(uint32_t idx, const Vec2& val)
{
  vertexBuffer->set(idx, UT_texcoord0, val);
}

Vec2 Mesh::getVertex(uint32_t idx)
{
  return vertexBuffer->getAsVec2(idx, UT_position);
}

void Mesh::set(uint32_t idx, UsageType ut, uint8_t val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, uint16_t val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, uint32_t val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, float val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, const Vec2& val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, const Vec3& val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, const Vec4& val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, const Color& val) { bufferFromUsageType(ut)->set(idx, ut, val); }

Vec2 Mesh::getAsVec2(uint32_t idx, UsageType ut) { return bufferFromUsageType(ut)->getAsVec2(idx, ut); }
Vec3 Mesh::getAsVec3(uint32_t idx, UsageType ut) { return bufferFromUsageType(ut)->getAsVec3(idx, ut); }
uint32_t   Mesh::getAsU32(uint32_t idx, UsageType ut) { return bufferFromUsageType(ut)->getAsU32(idx, ut); }
uint16_t   Mesh::getAsU16(uint32_t idx, UsageType ut) { return bufferFromUsageType(ut)->getAsU16(idx, ut); }

MeshPtr Mesh::clone()
{
  MeshPtr result(new Mesh);
  
  result->material = material;
  result->transform = transform;
  result->vertexBuffer = vertexBuffer;
  result->indexBuffer = indexBuffer;
  
  return result;
}

uint32_t Mesh::numVertices()
{
  return vertexBuffer->hostBuffer->count; 
}

uint32_t Mesh::numIndices()
{
  return indexBuffer->hostBuffer->count;
}

}

