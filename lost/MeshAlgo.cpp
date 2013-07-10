#include "lost/MeshAlgo.h"
#include "Mesh.h"
#include "HybridIndexBuffer.h"
#include "HybridVertexBuffer.h"

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

void catmullRom(const vector<Vec2>& controlPoints,  // arbitrary number of control point, at least 4
                u32 numInterpolatedPoints,          // number of points to calculate from the controlPoints
                vector<Vec2>& interpolatedPoints)    // result vector, will be resized to numInterpolatedPoints
{
  interpolatedPoints.resize(numInterpolatedPoints);
  // a minimum of 4 control points is required for the initial segment.
  // Each consecutive is made up by following point + 3 previous.
  uint32_t numSegments = ((uint32_t)controlPoints.size()-4)+1;
  vector<uint32_t> pn; // number of points per segment
  pn.resize(numSegments);

  // vertices spread evenly between segments
  // leftovers are attached to last segment
  uint32_t nps = numInterpolatedPoints / numSegments;
  uint32_t pbudget = numInterpolatedPoints;
  for(uint32_t i=0; i<numSegments; ++i)
  {
    if(2*nps > pbudget)
    {
      pn[i] = pbudget; // should only be the last one
    }
    else
    {
      pn[i] = nps;
      pbudget -= nps;
    }
//    DOUT("seg "<<pn[i]);
  }
  
  uint32_t pointOffset = 0;
  for(uint32_t segnum = 0; segnum < numSegments; ++segnum)
  {
    catmullRomSegment(interpolatedPoints, pointOffset, pn[segnum], controlPoints, segnum);
    pointOffset += pn[segnum];
  }

}

void calculateNormals(const vector<Vec2>& points, // vector of points that form a line
                      vector<Vec2>& normals)
{
  u32 num = (u32)points.size();
  normals.resize(num);
  // fix normal vectors
  ASSERT(num >= 2, "numVertices must be at least 2");
  for(u32 i=0; i<(num-1); ++i)
  {
    Vec2 tv = points[i+1] - points[i+0];
    Vec2 tnv;
    tnv.x = -tv.y;
    tnv.y = tv.x;
    normalise(tnv);
    normals[i] = tnv;
  }
  normals[num-1] = normals[num-2];  
}


}
