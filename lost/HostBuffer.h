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

#ifndef LOST_GL_HOSTBUFFER_H
#define LOST_GL_HOSTBUFFER_H

#include "lost/BufferLayout.h"

namespace lost
{

struct HostBuffer
{
  BufferLayout  layout;
  uint8_t*      buffer; // the actual physical buffer
  uint32_t      count; // number of vertices/indices in this HostBuffer
  
  void init(const BufferLayout& inLayout);
  HostBuffer(const BufferLayout& inLayout);
  HostBuffer(const BufferLayout& inLayout, uint32_t num);
  virtual ~HostBuffer();
  void deleteBuffer();
  uint32_t bufferSize(); // returns buffersize in bytes calculated from bufferlayout isze and count
  
  
  // resizes the buffer to accomodate num structs with the current layout. if num is 0, the buffer will be cleared
  void reset(uint32_t num);
  void reset(DataPtr data);
  unsigned char* elementAddress(uint32_t idx, UsageType ut); 
  ElementType elementTypeFromUsageType(UsageType ut);
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
  uint32_t   getAsU32(uint32_t idx, UsageType ut); 
  uint16_t   getAsU16(uint32_t idx, UsageType ut);
  bool hasUsageType(UsageType ut); // true if there is an attribute with that usage type, false otherwise
  
  uint32_t numScalarsForUsageType(UsageType ut);
private:
  DISALLOW_COPY_AND_ASSIGN(HostBuffer);
};


}

#endif
