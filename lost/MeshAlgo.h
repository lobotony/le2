#ifndef LOST_MESHALGO_H
#define LOST_MESHALGO_H

namespace lost
{
  MeshPtr newTriangleStrip(u32 numTriangles);
  MeshPtr newLineStrip(uint16_t numVerts);
  MeshPtr newLineGroup(uint16_t numLines);

Vec2 catmullRomInterpolate(f32 t, const Vec2& cp0, const Vec2& cp1, const Vec2& cp2, const Vec2& cp3);

void catmullRomSegment(vector<Vec2>&        interpolated, // receives the interpolated points
                         uint32_t             pointOffset,  // offset write position into interpolated points. The current segment points will be written at pointOffset onwards
                         uint32_t             numPoints,    // number of points for this segment
                         const vector<Vec2>&  cp,           // control points (all of them)
                         uint32_t             cpoffset);     // offset into control points. Four points after and including cp[0+cpoffset] will be used

}

#endif

