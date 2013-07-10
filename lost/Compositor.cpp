#include "lost/Compositor.h"
#include "lost/Canvas.h"
#include "lost/Quad.h"
#include "lost/Application.h"
#include "lost/ResourceManager.h"
#include "lost/Camera2D.h"
#include "lost/HybridBuffer.h"
#include "lost/DrawContext.h"
#include "lost/layers/Layer.h"
#include "lost/FrameBuffer.h"
#include "lost/Context.h"
#include <algorithm>

namespace lost
{

Compositor::Compositor()
{
  drawContext = new DrawContext(Application::instance()->glContext);
  fb.reset(new FrameBuffer);
  fbcam.reset(new Camera2D(Rect(0, 0, 0, 0)));
  uicam.reset(new Camera2D(Rect(0, 0, 0, 0)));
}

Compositor::~Compositor()
{
}
  
void Compositor::windowResized(const Vec2& newSize)
{
  DOUT("");
  windowSize = newSize;
  uicam->viewport(Rect(0,0,windowSize));
}
  
void Compositor::draw(const LayerPtr& rootLayer)
{
  prepareRedraws(rootLayer);
  updateLayerCaches();

  drawContext->glContext->bindDefaultFramebuffer();
  drawContext->glContext->camera(uicam);
  drawContext->drawTexturedRect(rootLayer->rect(), layerCache[rootLayer.get()], whiteColor);
  
  redrawCandidates.clear();
  redraws.clear();
}

void Compositor::prepareRedraws(const LayerPtr rootLayer)
{
  if(redrawCandidates.size()>0)
  {
    // remove all candidates that are currently set to invisible or not part of the main hierarchy that starts at root layer
    DOUT("redraw candidates: "<<u64(redrawCandidates.size()));
    for(auto layer : redrawCandidates)
    {
      if(layer->isVisibleWithinSuperlayers() && layer->isSublayerOf(rootLayer.get()))
      {
        redraws.push_back(layer);
      }
    }
    
    // sort the remaining redraws by z coordinate, descending
    std::sort(redraws.begin(), redraws.end(), [](Layer* l1, Layer* l2){ return l1->z() > l2->z(); });
  }  
}

void Compositor::updateLayerCaches()
{
  for(Layer* layer : redraws)
  {
    DOUT(layer->z() << " : " << layer->description());
    // find existing texture for layer or create new one and resize to current layer size
    TexturePtr texture;
    auto pos = layerCache.find(layer);
    if(pos != layerCache.end())
    {
      texture = pos->second;
      texture->init(layer->rect().size());
    }
    else
    {
      texture.reset(new Texture(layer->rect().size()));
      layerCache[layer] = texture;
    }
    
    // set up frame buffer for layer cache
    fb->bind();
    fb->detachAll();
    fb->attachColorBuffer(0, texture);
    fb->check();
    fbcam->viewport(Rect(0,0,layer->rect().size()));
    drawContext->glContext->camera(fbcam);
    // draw current layer contents. does NOT draw sublayers
    layer->draw(drawContext);
    
    // draw sublayer contents
    for(LayerPtr sublayer : layer->sublayers)
    {
      drawContext->drawTexturedRect(sublayer->rect(), layerCache[sublayer.get()], whiteColor);
    }
  }
}

void Compositor::needsRedraw(Layer* layer)
{
  auto pos = find(redrawCandidates.begin(), redrawCandidates.end(), layer);
  if(pos == redrawCandidates.end())
  {
    redrawCandidates.push_back(layer);
  }
}

}

