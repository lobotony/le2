#ifndef GUIRO_RENDERSYSTEM_H
#define GUIRO_RENDERSYSTEM_H

#include "guiro/views/View.h"
#include "lost/Context.h"

namespace lost
{

struct RenderContext;

struct RenderSystem
{
  RenderSystem();
  ~RenderSystem();
  
  void draw(Context* glContext, const ViewPtr& rootView);
  
  void windowResized(const Vec2& newSize);
  
private:
  CanvasPtr canvas;
  QuadPtr canvasQuad;
  Camera2DPtr uicam;
  
  RenderContext* rc;
  
  void draw(Context* glContext, const LayerPtr& layer);
};
}

#endif

