#pragma once

#include "lost/Application.h"

namespace lost
{

struct SunApp : Application
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
  
  TexturePtr splineTexture;

  f32 d;
  f32 splineWidth;
  
  CanvasPtr offscreenCanvas;
  CanvasPtr hblurCanvas;
  CanvasPtr vblurCanvas;
  QuadPtr canvasQuad;
  
  std::function<void(void)> sceneRenderFunc; // renders only the scene
  std::function<void(void)> mainRenderFunc; // render scene and postprocesing
  
  
  ///////////////////
  
  f32 o1;
  f32 o2;
  f32 o3;
  
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
  void setupSplineTexture();
};

}


