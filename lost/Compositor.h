#pragma once

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
  
  void layerDying(Layer* layer);
  void clearCacheForLayer(Layer* layer);
  void reset(); // called when ui is disabled, clears and resets all state

  void cachedDraw(const LayerPtr& rootLayer);
  void unchachedDraw(const LayerPtr& rootLayer);

  void logCacheStats();

private:
  Vec2 windowSize;
  Camera2DPtr uicam;
  
  FrameBufferPtr fb;
  Camera2DPtr fbcam;
  
  
  DrawContext* drawContext;
  
  void checkedNeedsRedraw(Layer* layer);
  void prepareRedraws(const LayerPtr rootLayer);
  void updateLayerCaches();
  
  vector<Layer*> redrawCandidates;
  vector<Layer*> redraws;
  map<Layer*, TexturePtr> layerCache;
  
  // uncached drawing
  TexturePtr drawBuffer;
  void unchachedDraw(Vec2 pos, const LayerPtr& layer);
  void drawLayer(const Vec2& globalLayerOrigin, const LayerPtr& layer);
  u32 numDraws;
  
};
}


