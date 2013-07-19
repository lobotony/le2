#ifndef LOST_MESHALGO_H
#define LOST_MESHALGO_H

namespace lost
{
  MeshPtr newTriangleStrip(u32 numTriangles);
  MeshPtr newLineStrip(uint16_t numVerts);
  MeshPtr newLineGroup(uint16_t numLines);

  Vec2 catmullRomInterpolate(f32 t, const Vec2& cp0, const Vec2& cp1, const Vec2& cp2, const Vec2& cp3);

  Vec2 bezierInterpolate(f32 t, const Vec2& cp0, const Vec2& cp1, const Vec2& cp2, const Vec2& cp3);

  void catmullRomSegment(vector<Vec2>&        interpolated, // receives the interpolated points
                           uint32_t             pointOffset,  // offset write position into interpolated points. The current segment points will be written at pointOffset onwards
                           uint32_t             numPoints,    // number of points for this segment
                           const vector<Vec2>&  cp,           // control points (all of them)
                           uint32_t             cpoffset);     // offset into control points. Four points after and including cp[0+cpoffset] will be used

  void catmullRom(const vector<Vec2>& controlPoints,  // arbitrary number of control point, at least 4
                  u32 numInterpolatedPoints,          // number of points the result will contain
                  vector<Vec2>& interpolatedPoints);   // result vector, will be resized to numInterpolatedPoints

  void calculateNormals(const vector<Vec2>& points, // vector of points that form a line
                        vector<Vec2>& normals);     //receives normal vectors calculated from points
}

#endif

