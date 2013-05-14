#ifndef LOST_DEMOENGINE_H
#define LOST_DEMOENGINE_H

#include "lost/Engine.h"

namespace lost
{

struct DemoEngine : Engine
{
  void startup();
  void update();
  void shutdown();

  void updateSpline(const vector<Vec2>& cp,
                    MeshPtr& lineMesh,
                    MeshPtr& normalMesh,
                    MeshPtr& triangles);

  void updateDeltaTime();
  
  MeshPtr coloredQuad;
  MeshPtr texturedQuad;
  ShaderProgramPtr colorShader;
  ShaderProgramPtr textureShader;
  ShaderProgramPtr pointShader;
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

  f64 lastTime;
  f64 nowTime;
  f64 deltaTime;

  vector<Vec2> controlPoints;
  vector<Vec2> cp2;
};

}

#endif

