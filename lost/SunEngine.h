#ifndef LOST_SUNENGINE_H
#define LOST_SUNENGINE_H

#include "lost/Engine.h"

namespace lost
{

struct SunEngine : Engine
{
  void startup();
  void update();
  void shutdown();

  void updateSpline(const vector<Vec2>& cp,
                    u32 numPoints,
                    MeshPtr& triangles);
  
  ShaderProgramPtr colorShader;
  ShaderProgramPtr textureShader;
  ShaderProgramPtr hblurShader;
  ShaderProgramPtr vblurShader;
  
  CameraPtr cam;

  u32 numInterpolatedPoints;
  
  MeshPtr triangulatedSpline;
  
  TexturePtr splineTexture;

  vector<Vec2> controlPoints;
  vector<Vec2> cp2;
  f32 d;
  
  FrameBufferPtr fb0;
  QuadPtr fb0quad;

  FrameBufferPtr fb1;
  QuadPtr fb1quad;

  CameraPtr fbcam;
  
  u32 dotsize;
  f32 splineWidth;
  
  std::function<void(void)> sceneRenderFunc; // renders only the scene
  std::function<void(void)> mainRenderFunc; // render scene and postprocesing
  
};

}

#endif

