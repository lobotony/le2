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
  
  void draw(const ViewPtr& rootView);
  
  void windowResized(const Vec2& newSize);
  
  void needsRedraw(Layer* layer);
  
  
private:
  CanvasPtr canvas;
  QuadPtr canvasQuad;
  Camera2DPtr uicam;
  
  RenderContext* rc;
  
  void draw(const LayerPtr& layer);
  
  vector<Layer*> redraws;
  map<Layer*, TexturePtr> layerCache;
};
}

#endif

