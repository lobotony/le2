#include "guiro/RenderSystem.h"
#include "lost/Canvas.h"
#include "lost/Quad.h"
#include "lost/Application.h"
#include "lost/ResourceManager.h"
#include "lost/Camera2D.h"
#include "lost/HybridIndexBuffer.h"
#include "guiro/RenderContext.h"
#include "guiro/layers/Layer.h"
#include "lost/FrameBuffer.h"
#include "lost/Context.h"

namespace lost
{
RenderSystem::RenderSystem()
{
  rc = new RenderContext(Application::instance()->glContext);
  fb.reset(new FrameBuffer);
  fbcam.reset(new Camera2D(Rect(0, 0, 0, 0)));
}

RenderSystem::~RenderSystem()
{
}
  
void RenderSystem::windowResized(const Vec2& newSize)
{
  DOUT("");
/*  canvas.reset(new Canvas(newSize));
  canvasQuad = Quad::create(canvas->texture(), false);
  canvasQuad->material->shader = Application::instance()->resourceManager->shader("resources/glsl/texture");
  canvasQuad->material->blendPremultiplied();
  uicam.reset(new Camera2D(Rect(0,0,newSize)));*/
  windowSize = newSize;
}
  
void RenderSystem::draw(const LayerPtr& rootLayer)
{
  prepareRedraws(rootLayer);
  updateLayerCaches();
  redraw();

/*  if(rootView)
  {
    draw(rootView->layer);
    rc->glContext->bindDefaultFramebuffer();
    rc->glContext->camera(uicam);
    rc->glContext->draw(canvasQuad);
  }*/
  
  redrawCandidates.clear();
  redraws.clear();
}

void RenderSystem::prepareRedraws(const LayerPtr rootLayer)
{
  if(redrawCandidates.size()>0)
  {
    DOUT("redraw candidates: "<<u64(redrawCandidates.size()));
    for(auto layer : redrawCandidates)
    {
      if(layer->isVisibleWithinSuperlayers() && layer->isSublayerOf(rootLayer.get()))
      {
        redraws.push_back(layer);
      }
    }
  }  
}

void RenderSystem::updateLayerCaches()
{
  for(Layer* layer : redraws)
  {
    // find existing texture for layer or create new one and resize to current layer size
    TexturePtr texture;
    auto pos = layerCache.find(layer);
    if(pos != layerCache.end())
    {
      texture = pos->second;
      texture->init(layer->rect.size());
    }
    else
    {
      texture.reset(new Texture(layer->rect.size()));
      layerCache[layer] = texture;
    }
    
    // set up frame buffer
    fb->bind();
    fb->detachAll();
    fb->attachColorBuffer(0, texture);
    fb->check();
    fbcam->viewport(Rect(0,0,layer->rect.size()));
    rc->glContext->camera(fbcam);
    layer->draw(rc);
  }
}

void RenderSystem::redraw()
{
  
}

/*void RenderSystem::draw(const LayerPtr& layer)
{
  canvas->drawToCanvas([this, layer](){
    rc->drawSolidRect(layer->rect, layer->backgroundColor);
  });
}*/

void RenderSystem::needsRedraw(Layer* layer)
{
  redrawCandidates.push_back(layer);
}

}

