#ifndef GUIRO_RENDERSYSTEM_H
#define GUIRO_RENDERSYSTEM_H

#include "guiro/types.h"

namespace lost
{

struct RenderContext;
struct Layer;

struct RenderSystem
{
  RenderSystem();
  ~RenderSystem();
  
  void draw(const LayerPtr& rootLayer);
  
  void windowResized(const Vec2& newSize);
  
  void needsRedraw(Layer* layer);
  
  
private:
  Vec2 windowSize;
  CanvasPtr canvas;
  QuadPtr canvasQuad;
  Camera2DPtr uicam;
  
  FrameBufferPtr fb;
  Camera2DPtr fbcam;
  
  
  RenderContext* rc;
    
  void prepareRedraws(const LayerPtr rootLayer);
  void updateLayerCaches();
  void redraw();
  
  vector<Layer*> redrawCandidates;
  vector<Layer*> redraws;
  map<Layer*, TexturePtr> layerCache;
};
}

#endif

