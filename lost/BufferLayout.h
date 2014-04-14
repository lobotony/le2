// FIXME: rename to vertexlayout

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

namespace lost
{


// these are the basic elements that can be used to define a buffer layout
typedef uint32_t ElementType;
const static ElementType ET_u8 = 0;
const static ElementType ET_u16 = 1;
const static ElementType ET_u32 = 2;
const static ElementType ET_f32 = 3;

const static ElementType ET_vec2_u8 = 10;
const static ElementType ET_vec2_u16 = 11;
const static ElementType ET_vec2_u32 = 12;
const static ElementType ET_vec2_f32 = 13;

const static ElementType ET_vec3_u8 = 20;
const static ElementType ET_vec3_u16 = 21;
const static ElementType ET_vec3_u32 = 22;
const static ElementType ET_vec3_f32 = 23;

const static ElementType ET_vec4_u8 = 30;
const static ElementType ET_vec4_u16 = 31;
const static ElementType ET_vec4_u32 = 32;
const static ElementType ET_vec4_f32 = 33;

const static ElementType ET_mat4x4_f32 = 40;


uint32_t elementSize(ElementType t); // returns elemtn size in bytes
uint32_t numScalarsForElementType(ElementType et); // returns the number of scalars an ET is made of. 
                                                   // This is mostly relevant for vectpr/matrix since all other types  1.
GLenum   glScalarTypeForElementType(ElementType et);
// each element or stride of a buffer needs to be marked with a usage type, so it can be mapped to a GL buffer and stride
typedef uint32_t UsageType;
const static UsageType UT_unused = 0;  // elements marked with this usage type will be ignored, handy for testing or padding to multiples of 32/64 bytes for performance optimisation
const static UsageType UT_index = 1;       // special case for IndexBuffers, the element is an index into a group of vertices, used for drawing
const static UsageType UT_position = 2;      // element is a vertex coordinate, usually a vec2/3/4 variant
const static UsageType UT_normal = 3;      // element is a normal vector
const static UsageType UT_color = 4;       // element is a color, usually a vec3/vec4 variant
const static UsageType UT_texcoord0 = 5;   // element is a texture coordinate and should be mapped to texture unit 0
const static UsageType UT_texcoord1 = 6;   
const static UsageType UT_texcoord2 = 7;   
const static UsageType UT_texcoord3 = 8;   
const static UsageType UT_texcoord4 = 9;   
const static UsageType UT_texcoord5 = 10;   
const static UsageType UT_texcoord6 = 11;   
const static UsageType UT_texcoord7 = 12;   
const static UsageType UT_texcoord8 = 13;   
const static UsageType UT_texcoord9 = 14;   
const static UsageType UT_texcoord10 = 15;  
const static UsageType UT_texcoord11 = 16;   
const static UsageType UT_texcoord12 = 17;   
const static UsageType UT_texcoord13 = 18;   
const static UsageType UT_texcoord14 = 19;   
const static UsageType UT_texcoord15 = 20;   
const static UsageType UT_vertexAttrib0 = 21; // element should be mapped to vertex attribute 0 of a shader program
const static UsageType UT_vertexAttrib1 = 22; 
const static UsageType UT_vertexAttrib2 = 23; 
const static UsageType UT_vertexAttrib3 = 24; 
const static UsageType UT_vertexAttrib4 = 25; 
const static UsageType UT_vertexAttrib5 = 26; 
const static UsageType UT_vertexAttrib6 = 27; 
const static UsageType UT_vertexAttrib7 = 28; 
const static UsageType UT_vertexAttrib8 = 29; 
const static UsageType UT_vertexAttrib9 = 30; 
const static UsageType UT_vertexAttrib10 = 31; 
const static UsageType UT_vertexAttrib11 = 32; 
const static UsageType UT_vertexAttrib12 = 33; 
const static UsageType UT_vertexAttrib13 = 34; 
const static UsageType UT_vertexAttrib14 = 35; 
const static UsageType UT_vertexAttrib15 = 36; 

const char* nameForElementType(ElementType et);
const char* nameForUsageType(UsageType ut);

// elements must be paired with usage types
struct BufferLayoutAttribute
{
  ElementType elementType;
  UsageType   usageType;
  bool        normalise;
  
  BufferLayoutAttribute(ElementType et, UsageType ut) : elementType(et), usageType(ut), normalise(false) {}
  BufferLayoutAttribute(ElementType et, UsageType ut, bool inNormalise) : elementType(et), usageType(ut), normalise(inNormalise) {}
  
  uint32_t size() const { return elementSize(elementType);}
};

struct AttributePointerConfig
{
  AttributePointerConfig() : size(0), type(0), stride(0), offset(0), normalise(0) {}

  GLint   size;
  GLenum  type;
  GLsizei stride;
  GLvoid* offset;
  GLboolean normalise;
};

struct BufferLayout
{
  // FIXME: add caching for various size and partition parameters so we don'T have to recalculate on every set call
  // add maps that hold these values. Update maps upon each add call.
  vector<BufferLayoutAttribute> attributes;
  map<UsageType, ElementType> ut2et;
  map<UsageType, AttributePointerConfig> ut2apc;
  
  void updatePointerConfigs();
  void add(ElementType elementType, UsageType usageType);
   
  // size of the whole layout in bytes, without taking partitioning into account, in bytes
  uint32_t size() const;
  uint32_t numScalarsForUsageType(UsageType ut);
//  uint32_t structSize() const;
  
  const AttributePointerConfig& pointerConfigForUsageType(UsageType ut) { return ut2apc[ut]; };
  
  bool hasUsageType(UsageType ut);
  GLenum glScalarTypeFromUsageType(UsageType ut);
  uint32_t stride(UsageType ut);
  size_t offset(UsageType ut);
};

} // lost

