#include "guiro/RenderSystem.h"
#include "lost/Canvas.h"
#include "lost/Quad.h"
#include "lost/Application.h"
#include "lost/ResourceManager.h"
#include "lost/Camera2D.h"
#include "lost/HybridIndexBuffer.h"
#include "guiro/RenderContext.h"

namespace lost
{
RenderSystem::RenderSystem()
{
  rc = new RenderContext(Application::instance()->glContext);
  
  // calling windowResized here prevents the render system from starting up with null framebuffer
  // we have to use 1,1 as minimum since a 0,0 framebuffer will produce GL errors should it get used. 
  windowResized(Vec2(1,1));
}

RenderSystem::~RenderSystem()
{
}
  
void RenderSystem::windowResized(const Vec2& newSize)
{
  DOUT("");
  canvas.reset(new Canvas(newSize));
  canvasQuad = Quad::create(canvas->texture(), false);
  canvasQuad->material->shader = Application::instance()->resourceManager->shader("resources/glsl/texture");
  canvasQuad->material->blendPremultiplied();
  uicam.reset(new Camera2D(Rect(0,0,newSize)));
}
  
void RenderSystem::draw(const LayerPtr& rootLayer)
{
  prepareRedraws();
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

void RenderSystem::prepareRedraws()
{
  if(redrawCandidates.size()>0)
  {
    DOUT("redraw candidates: "<<u64(redrawCandidates.size()));
    for(auto layer : redrawCandidates)
    {
      
    }
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

