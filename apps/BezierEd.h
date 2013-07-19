//
//  BezierEd.h
//  LostEngine2
//
//  Created by Tony Kostanjsek on 19.07.13.
//  Copyright (c) 2013 Tony Kostanjsek. All rights reserved.
//

#ifndef __LostEngine2__BezierEd__
#define __LostEngine2__BezierEd__

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

#endif /* defined(__LostEngine2__BezierEd__) */
