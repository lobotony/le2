#ifndef LOST_DEMOAPP_H
#define LOST_DEMOAPP_H

#include "lost/Application.h"

namespace lost
{

struct DemoApp : Application
{
  void startup();
  void update();
  void shutdown();

  void updateSpline(const vector<Vec2>& cp,
                    MeshPtr& lineMesh,
                    MeshPtr& normalMesh,
                    MeshPtr& triangles);
  
  MeshPtr coloredQuad;
  MeshPtr texturedQuad;
  ShaderProgramPtr colorShader;
  ShaderProgramPtr textureShader;
  ShaderProgramPtr hblurShader;
  ShaderProgramPtr vblurShader;
  
  CameraPtr cam;
  TexturePtr ringTexture;
  FontPtr font;
  TextMeshPtr rt1;
  TextMeshPtr rt2;
  TextMeshPtr rt3;

  MeshPtr dot;
  MeshPtr dot2;

  MeshPtr lines;
  MeshPtr spline;
  MeshPtr normals;
  MeshPtr triangulatedSpline;

  vector<MeshPtr> cpdots;
  vector<MeshPtr> ipdots;
  
  TexturePtr splineTexture;

  vector<Vec2> controlPoints;
  vector<Vec2> cp2;
  f32 d;
  
  FrameBufferPtr fb0;
  QuadPtr fb0quad;

  FrameBufferPtr fb1;
  QuadPtr fb1quad;

  CameraPtr fbcam;
};

}

#endif

