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

void Compositor::layerDying(Layer* layer)
{
//  DOUT(layer->name);
  
  // remove from all containers that don't own layer to prevent dangling pointers
  // remove cache to improve resource usage
  
  // redrawCandidates
  auto pos = find(redrawCandidates.begin(), redrawCandidates.end(), layer);
  if(pos != redrawCandidates.end())
  {
    redrawCandidates.erase(pos);
    DOUT("removing layer from redrawCandidates: "<<layer->name);
  }

  // redraws
  auto pos2 = find(redraws.begin(), redraws.end(), layer);
  if(pos2 != redraws.end())
  {
    redraws.erase(pos2);
    DOUT("removing layer from redraws: "<<layer->name);
  }

  clearCacheForLayer(layer);
}

void Compositor::reset()
{
  redrawCandidates.clear();
  redraws.clear();
  layerCache.clear();
  fb->bind();
  fb->detachAll();
  drawContext->glContext->bindDefaultFramebuffer();
}

void Compositor::clearCacheForLayer(Layer* layer)
{
  auto pos = layerCache.find(layer);
  if(pos != layerCache.end())
  {
    layerCache.erase(pos);
//    DOUT("removing cache for layer: "<<layer->name);
  }
}
  
void Compositor::windowResized(const Vec2& newSize)
{
  windowSize = newSize;
  uicam->viewport(Rect(0,0,windowSize));
}
  
void Compositor::draw(const LayerPtr& rootLayer)
{
  cachedDraw(rootLayer);
//  unchachedDraw(rootLayer);
  
  redrawCandidates.clear();
  redraws.clear();
}

#pragma mark - uncached draw -

void Compositor::unchachedDraw(const LayerPtr& rootLayer)
{
  numDraws=0;
  unchachedDraw(Vec2(0,0), rootLayer);
//  DOUT("layers drawn: "<<numDraws);
}

void Compositor::unchachedDraw(Vec2 pos, const LayerPtr& layer)
{
  drawLayer(pos, layer);
  for(auto sublayer : layer->sublayers)
  {
    unchachedDraw(pos+sublayer->pos(), sublayer);
  }
}

void Compositor::drawLayer(const Vec2& globalLayerOrigin, const LayerPtr& layer)
{
  numDraws++;
  // create framebuffer texture if required
  if(!drawBuffer)
  {
    drawBuffer.reset(new Texture);
  }
  drawBuffer->init(layer->rect().size());

  // setup framebuffer and camera
  fb->bind();
  fb->detachAll();
  fb->attachColorBuffer(0, drawBuffer);
  fb->check(); // FIXME: remove this once it is not needed anymore for debugging
  fbcam->viewport(Rect(0,0,layer->rect().size()));
  drawContext->glContext->camera(fbcam);

  // draw layer into texture
  layer->draw(drawContext);

  drawContext->glContext->bindDefaultFramebuffer();  
  drawContext->glContext->camera(uicam);
  // draw texture as rect onto screen at global origin pos
  Color drawColor(1.0f, 1.0f, 1.0f, layer->opacity());
  drawContext->drawTexturedRect(Rect(globalLayerOrigin, layer->rect().size()), drawBuffer, drawColor);
}

#pragma mark - cached draw -

void Compositor::cachedDraw(const LayerPtr& rootLayer)
{
  numDraws = 0;
  prepareRedraws(rootLayer);
  updateLayerCaches();

  drawContext->glContext->bindDefaultFramebuffer();
  drawContext->glContext->camera(uicam);
  Color drawColor(1.0f, 1.0f, 1.0f, rootLayer->opacity());
  drawContext->drawTexturedRect(rootLayer->rect(), layerCache[rootLayer.get()], drawColor);
//  DOUT("layer caches drawn: "<<numDraws);
}

void Compositor::prepareRedraws(const LayerPtr rootLayer)
{
  if(redrawCandidates.size()>0)
  {
    // remove all candidates that are currently set to invisible or not part of the main hierarchy that starts at root layer
//    DOUT("redraw candidates: "<<u64(redrawCandidates.size()));
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
  Clock c;
  for(Layer* layer : redraws)
  {
    numDraws++;
//    DOUT(layer->z() << " : " << layer->description());
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
    fb->check(); // FIXME: remove this once it is not needed anymore for debugging
    fbcam->viewport(Rect(0,0,layer->rect().size()));
    drawContext->glContext->camera(fbcam);
    // draw current layer contents. does NOT draw sublayers
    layer->draw(drawContext);
    
    // draw sublayer contents
    for(LayerPtr sublayer : layer->sublayers)
    {
      if(sublayer->visible())
      {
        Color drawColor(1.0f,1.0f,1.0f, sublayer->opacity());
        const TexturePtr& tex = layerCache[sublayer.get()];
        if(tex)
        {
          drawContext->drawTexturedRect(sublayer->rect(), tex, drawColor);
        }
      }
    }
  }
  if(redraws.size()>0)
  {
    c.update();
//    DOUT("redraw "<<(s64)redraws.size()<<" in "<<c.deltaStart<<" sec");
  }
}

#pragma mark - redraw scheduling -

void Compositor::needsRedraw(Layer* layer)
{
  while(layer)
  {
    checkedNeedsRedraw(layer);
    layer = layer->superlayer;
  }
}

void Compositor::checkedNeedsRedraw(Layer* layer)
{
  auto pos = find(redrawCandidates.begin(), redrawCandidates.end(), layer);
  if(pos == redrawCandidates.end())
  {
    redrawCandidates.push_back(layer);
  }
}

void Compositor::logCacheStats()
{
  u32 mem = 0;
  for(auto i : layerCache)
  {
    mem += (i.second->dataWidth * i.second->dataHeight);
  }
  mem *= 4;
  mem /= 1024;
  DOUT("Layer cache: entries"<<(u32)layerCache.size()<<" approx. mem: "<<mem<<" kb");
}

}

