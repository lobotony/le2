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

#pragma once

#include "lost/BufferLayout.h"

namespace lost
{


/** base class for hybrid buffers that contain a hostbuffer as well as one or more
 *  gpu buffers.
 *  Only one type of gpu buffer can be managed by a hybrid buffer.
 */
struct HybridBuffer
{
  typedef map<UsageType, string> VertexAttributeMap;
  // we need to separate construction and initialisation because derived classes
  // have to perform more complex setup operations in constructor.
  HybridBuffer();               
  void init(GLenum inGpuBufferType,
            const BufferLayout& inLayout);               
  virtual ~HybridBuffer();
  
  void reset(uint32_t num);
  void reset(DataPtr data);
  
  void set(uint32_t idx, UsageType ut, uint8_t val);
  void set(uint32_t idx, UsageType ut, uint16_t val);
  void set(uint32_t idx, UsageType ut, uint32_t val);
  void set(uint32_t idx, UsageType ut, float val);
  void set(uint32_t idx, UsageType ut, const Vec2& val);
  void set(uint32_t idx, UsageType ut, const Vec3& val);
  void set(uint32_t idx, UsageType ut, const Vec4& val);
  void set(uint32_t idx, UsageType ut, const Color& val);  
  
  Vec2 getAsVec2(uint32_t idx, UsageType ut);
  Vec3 getAsVec3(uint32_t idx, UsageType ut);
  uint32_t getAsU32(uint32_t idx, UsageType ut);
  uint16_t getAsU16(uint32_t idx, UsageType ut);
  
  void upload();
  
  bool hasUsageType(UsageType ut);
  const AttributePointerConfig& pointerConfigForUsageType(UsageType ut);
  
  GLenum gpuBufferType; // should be either GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER
  bool dirty;
  HostBufferPtr hostBuffer;
  BufferPtr     gpuBuffer;
  VertexAttributeMap vertexAttributeMap; // maps a usageType to a shader vertex attribute name
};

struct HybridIndexBuffer : HybridBuffer
{
  // since an IndexBuffer is only an array of integers, we can simplify the 
  // layouting of the buffer by only requiring a single element type parameter.
  HybridIndexBuffer(ElementType et)
  {
    // an indexbuffer only ever has one attribute with usage type index in a single partition
    // only the element type can vary, to optimize the buffer for hardware requirements or
    // mesh sizes.
    BufferLayout layout;
    layout.add(et, UT_index);
    switch(et)
    {
      case ET_u8:type = GL_UNSIGNED_BYTE;break;
      case ET_u16:type = GL_UNSIGNED_SHORT;break;
      case ET_u32:type = GL_UNSIGNED_INT;break;
      default:
        ASSERT(false,"only u8, u16, u32 are allowed");
    }
    drawMode = GL_TRIANGLES;
    init(GL_ELEMENT_ARRAY_BUFFER, layout);
  }
  
  GLenum type; // required for the actual draw operation, derived in constructor
  GLenum drawMode; // GL_LINES, GL_TRIANGLES etc.
};

struct HybridVertexBuffer : HybridBuffer
{
  HybridVertexBuffer(const BufferLayout& inLayout)
  {
    init(GL_ARRAY_BUFFER, inLayout);
  }
  
};

}

