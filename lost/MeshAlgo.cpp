#include "lost/MeshAlgo.h"
#include "Mesh.h"
#include "HybridIndexBuffer.h"
#include "HybridVertexBuffer.h"
#include <cmath>

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

Vec2 catmullRomInterpolate(f32 t, const Vec2& cp0, const Vec2& cp1, const Vec2& cp2, const Vec2& cp3)
{
  f32 b0 = .5f*(-powf(t,3)+2*powf(t,2)-t);
  f32 b1 = .5f*(3*powf(t,3)-5*powf(t,2)+2);
  f32 b2 = .5f*(-3*powf(t,3)+4*powf(t,2)+t);
  f32 b3 = .5f*(powf(t,3) - powf(t,2));
  Vec2 pt = b0*cp0 + b1*cp1 + b2*cp2 + b3*cp3;
  return pt;
}

void catmullRomSegment(vector<Vec2>&        interpolated, // receives the interpolated points
                         uint32_t             pointOffset,  // offset write position into interpolated points. The current segment points will be written at pointOffset onwards
                         uint32_t             numPoints,    // number of points for this segment
                         const vector<Vec2>&  cp,           // control points (all of them)
                         uint32_t             cpoffset)     // offset into control points. Four points after and including cp[0+cpoffset] will be used
{
  Vec2 cp0 = cp[cpoffset+0];
  Vec2 cp1 = cp[cpoffset+1];
  Vec2 cp2 = cp[cpoffset+2];
  Vec2 cp3 = cp[cpoffset+3];

//  DOUT("seglen "<<len(cp2-cp1));

  f32 t = 0;
  f32 dt = 1.0f/(numPoints);
  for(uint32_t i=0; i<numPoints; ++i)
  {
    interpolated[pointOffset+i] = catmullRomInterpolate(t, cp0, cp1, cp2, cp3);
    t += dt;
  }
}


}
