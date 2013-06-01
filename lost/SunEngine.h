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
  
  Vec2 winSize;
  CameraPtr cam;
  
  TexturePtr splineTexture;

  f32 d;
  
  FrameBufferPtr fb0;
  QuadPtr fb0quad;

  FrameBufferPtr fb1;
  QuadPtr fb1quad;

  CameraPtr fbcam;
  
  f32 splineWidth;
  
  std::function<void(void)> sceneRenderFunc; // renders only the scene
  std::function<void(void)> mainRenderFunc; // render scene and postprocesing
  
  
  ///////////////////
  
  u32 numCircles;
  u32 numSplines;
  Vec2 circleCenter;
  u32 numInterpolatedPoints;
  f32 minRadius;
  f32 maxRadius;
  
  vector<f32> circleRadius;
  Vec2* circlePoints;

  u32 dotSize;
  vector<MeshPtr> splines;
  vector<MeshPtr> circleDots;
  
  void fbsetup();
  void setupSplines();
  void updateSplines();
};

}

#endif

