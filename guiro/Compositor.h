#ifndef GUIRO_COMPOSITOR_H
#define GUIRO_COMPOSITOR_H

#include "guiro/types.h"

namespace lost
{

struct DrawContext;
struct Layer;

struct Compositor
{
  Compositor();
  ~Compositor();
  
  void draw(const LayerPtr& rootLayer);
  
  void windowResized(const Vec2& newSize);
  
  void needsRedraw(Layer* layer);
  
  
private:
  Vec2 windowSize;
  Camera2DPtr uicam;
  
  FrameBufferPtr fb;
  Camera2DPtr fbcam;
  
  
  DrawContext* drawContext;
    
  void prepareRedraws(const LayerPtr rootLayer);
  void updateLayerCaches();
  
  vector<Layer*> redrawCandidates;
  vector<Layer*> redraws;
  map<Layer*, TexturePtr> layerCache;
};
}

#endif

