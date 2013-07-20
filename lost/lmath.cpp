#include "lost/lmath.h"

namespace lost
{

Vec2 catmullRomInterpolate(f32 t, const Vec2& cp0, const Vec2& cp1, const Vec2& cp2, const Vec2& cp3)
{
  f32 b0 = .5f*(-powf(t,3)+2*powf(t,2)-t);
  f32 b1 = .5f*(3*powf(t,3)-5*powf(t,2)+2);
  f32 b2 = .5f*(-3*powf(t,3)+4*powf(t,2)+t);
  f32 b3 = .5f*(powf(t,3) - powf(t,2));
  Vec2 pt = b0*cp0 + b1*cp1 + b2*cp2 + b3*cp3;
  return pt;
}

Vec2 bezierInterpolate(f32 t, const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3)
{
  f32 u = 1.0f - t;
  f32 tt = t*t;
  f32 uu = u*u;
  f32 uuu = uu*u;
  f32 ttt = tt*t;
  
  Vec2 p = uuu*p0;
  p += 3*uu*t*p1;
  p += 3*u*tt*p2;
  p += ttt * p3;
  
  return p;
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
