#pragma once

#include "lost/Application.h"

namespace lost
{

union Event;

struct BezierEd : Application
{
  BezierEd();
  
  void startup();
  void update();
  void shutdown();

  ShaderProgramPtr colorShader;
  ShaderProgramPtr textureShader;
  Camera2DPtr cam;

  u32             numInterpolatedPoints;
  u32             splineWidth;
  f32             psize;
  Vec2            areaSize;

  MeshPtr         bezier;
  Vec2            offset;
  vector<ViewPtr> pv;
  vector<TextLayerPtr> labels;
  vector<Vec2>     p;

  FontPtr font;
  TexturePtr splineTexture;

  View* clickedView;

  std::function<void(Event*)> downHandler;
  std::function<void(Event*)> moveHandler;
  std::function<void(Event*)> upHandler;
  
  ViewPtr pointView(f32 x, f32 y);
  TextLayerPtr label(f32 x, f32 y);
  
  f32 labelWidth;
  f32 labelHeight;
  
  vector<Vec2> ip;
  vector<Vec2> nv;
  
  void pointsToViews();
  void updateSpline();
  void updateLabels();
  u32 indexFromView(View* v);
  void setupLabels();
  void setupSplineTexture();
};
}

