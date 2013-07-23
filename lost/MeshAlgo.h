#ifndef LOST_MESHALGO_H
#define LOST_MESHALGO_H

namespace lost
{
  MeshPtr newTriangleStrip(u32 numTriangles);
  MeshPtr newLineStrip(uint16_t numVerts);
  MeshPtr newLineGroup(uint16_t numLines);

}

#endif

