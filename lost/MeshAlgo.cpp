#include "lost/MeshAlgo.h"
#include "Mesh.h"
#include "lost/HybridBuffer.h"

namespace lost
{

MeshPtr newTriangleStrip(u32 numTriangles)
{
  ASSERT((numTriangles % 2) == 0, "numTriangles must be multiple of 2");
  
  MeshPtr result;

  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  layout.add(ET_vec2_f32, UT_texcoord0);  
  result = Mesh::create(layout, ET_u16);
  result->indexBuffer->drawMode = GL_TRIANGLE_STRIP;
  
  u32 numVertices = numTriangles + 2;
  u32 numIndices = numVertices;
  
/*  DOUT("-------------- newTriangleStrip");
  DOUT("numTriangles "<<numTriangles);
  DOUT("numVertices "<< numVertices);
  DOUT("numIndices "<< numIndices);*/
  
  result->vertexBuffer->reset(numVertices);
  result->indexBuffer->reset(numIndices);
  
  for(u32 i=0; i<numIndices; ++i)
  {
    result->set(i, UT_index, i);
  }
  
  return result;
}

MeshPtr newLineStrip(uint16_t numVerts)
{
  MeshPtr result;

  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  result = Mesh::create(layout, ET_u16);
  result->indexBuffer->drawMode = GL_LINE_STRIP;

  uint32_t numVertices = numVerts;
  uint32_t numIndices = numVertices;
  
  result->vertexBuffer->reset(numVertices);
  result->indexBuffer->reset(numIndices);
  
  for(uint16_t i=0; i<numIndices; ++i)
  {
    result->set(i, UT_index, i);
  }
  
  result->material->color = yellowColor;
  
  return result;
}

MeshPtr newLineGroup(uint16_t numLines)
{
  MeshPtr result;

  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  result = Mesh::create(layout, ET_u16);
  result->indexBuffer->drawMode = GL_LINES;

  uint32_t numVertices = numLines*2;
  uint32_t numIndices = numVertices;
  
  result->vertexBuffer->reset(numVertices);
  result->indexBuffer->reset(numIndices);
  
  for(uint16_t i=0; i<numIndices; ++i)
  {
    result->set(i, UT_index, i);
  }
  
  result->material->color = greenColor;
  
  return result;
}

}
