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

#include "lost/HostBuffer.h"
#include "lost/Data.h"

namespace lost
{

void HostBuffer::init(const BufferLayout& inLayout)
{
  layout = inLayout;
  buffer = NULL;
  count = 0;
}

HostBuffer::HostBuffer(const BufferLayout& inLayout)
{
  init(inLayout);
}

HostBuffer::HostBuffer(const BufferLayout& inLayout, uint32_t num)
{
  init(inLayout);
  reset(num);
}

HostBuffer::~HostBuffer()
{
  deleteBuffer();
}

void HostBuffer::deleteBuffer()
{
//    DOUT("deleting partition " << i);
  free(buffer);
  buffer = NULL;
}  

// resizes the buffer to accomodate num structs with the current layout. if num is 0, the buffer will be cleared
void HostBuffer::reset(uint32_t num)
{
  if(num == 0)
  {
    deleteBuffer();
  }
  else
  {
    deleteBuffer();
    uint32_t s = layout.size() *num;
    buffer = (uint8_t*)malloc(s);
//      DOUT("allocated p:"<<i<<" num:"<<num<<" bytes:"<<s);
  }
  count = num;
}

void HostBuffer::reset(DataPtr data)
{
  deleteBuffer();
  buffer = (uint8_t*)malloc(data->size);
  memcpy(buffer, data->bytes.get(), data->size);
  count = (uint32_t)(data->size / layout.size());
}
  
  
uint32_t HostBuffer::bufferSize()
{
  return layout.size()*count;
}

ElementType HostBuffer::elementTypeFromUsageType(UsageType ut)
{
  return layout.ut2et[ut];
}

uint8_t* HostBuffer::elementAddress(uint32_t idx, UsageType ut)
{
  uint8_t* result = 0;
  
  uint32_t size = layout.size();
  uint32_t offset = (uint32_t)layout.offset(ut);
  result = buffer + size*idx +offset; // this is the start of the vertex partition with the given index
  
  return result;
}

void HostBuffer::set(uint32_t idx, UsageType ut, uint8_t val)
{
  ElementType et = layout.ut2et[ut];
  switch(et)
  {
    case ET_u8:
    {
      uint8_t conv = val;
      memcpy(elementAddress(idx, ut), &conv, sizeof(conv));      
      break;
    }
    case ET_u16:
    {
      uint16_t conv = val;
      memcpy(elementAddress(idx, ut), &conv, sizeof(conv));      
      break;
    }
    case ET_u32:
    {
      uint32_t conv = val;
      memcpy(elementAddress(idx, ut), &conv, sizeof(conv));      
      break;
    }
    default:
      ASSERT(false, "can't set uint8_t for element "<<et);
      break;
  }
}

void HostBuffer::set(uint32_t idx, UsageType ut, uint16_t val)
{
  ElementType et = layout.ut2et[ut];
  switch(et)
  {
    case ET_u8:
    {
      uint8_t conv = (uint8_t)val;
      memcpy(elementAddress(idx, ut), &conv, sizeof(conv));      
      break;
    }
    case ET_u16:
    {
      uint16_t conv = val;
      memcpy(elementAddress(idx, ut), &conv, sizeof(conv));      
      break;
    }
    case ET_u32:
    {
      uint32_t conv = val;
      memcpy(elementAddress(idx, ut), &conv, sizeof(conv));      
      break;
    }
    default:
      ASSERT(false, "can't set uint16_t for element "<<et);
      break;
  }
}

void HostBuffer::set(uint32_t idx, UsageType ut, uint32_t val)
{
  ElementType et = layout.ut2et[ut];
  switch(et)
  {
    case ET_u8:
    {
      uint8_t conv = val;
      memcpy(elementAddress(idx, ut), &conv, sizeof(conv));      
      break;
    }
    case ET_u16:
    {
      uint16_t conv = val;
      memcpy(elementAddress(idx, ut), &conv, sizeof(conv));      
      break;
    }
    case ET_u32:
    {
      uint32_t conv = val;
      memcpy(elementAddress(idx, ut), &conv, sizeof(conv));      
      break;
    }
    default:
      ASSERT(false, "can't set uint32_t for element "<<et);
      break;
  }
}

void HostBuffer::set(uint32_t idx, UsageType ut, float val)
{
  memcpy(elementAddress(idx, ut), &val, sizeof(val));
}

void HostBuffer::set(uint32_t idx, UsageType ut, const Vec2& val)
{
  memcpy(elementAddress(idx, ut), &val, sizeof(val));
}

void HostBuffer::set(uint32_t idx, UsageType ut, const Vec3& val)
{
  memcpy(elementAddress(idx, ut), &val, sizeof(val));
}

void HostBuffer::set(uint32_t idx, UsageType ut, const Vec4& val)
{
  memcpy(elementAddress(idx, ut), &val, sizeof(val));
}

void HostBuffer::set(uint32_t idx, UsageType ut, const Color& val)
{
  memcpy(elementAddress(idx, ut), &val, sizeof(val));
}

Vec2 HostBuffer::getAsVec2(uint32_t idx, UsageType ut)
{
  // FIXME: this is completely unsafe
  return *((Vec2*)elementAddress(idx, ut));
}

Vec3 HostBuffer::getAsVec3(uint32_t idx, UsageType ut)
{
  // FIXME: this is completely unsafe
  return *((Vec3*)elementAddress(idx, ut));
}

uint32_t HostBuffer::getAsU32(uint32_t idx, UsageType ut)
{
  // FIXME: this is completely unsafe
  return *((uint32_t*)elementAddress(idx, ut));
}

uint16_t HostBuffer::getAsU16(uint32_t idx, UsageType ut)
{
  // FIXME: this is completely unsafe
  return *((uint16_t*)elementAddress(idx, ut));
}
    
bool HostBuffer::hasUsageType(UsageType ut)
{
  return layout.hasUsageType(ut);
}

uint32_t HostBuffer::numScalarsForUsageType(UsageType ut)
{
  ElementType et = layout.ut2et[ut];
  return numScalarsForElementType(et);
}

}
