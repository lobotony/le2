#include "guiro/RenderSystem.h"
#include "lost/Canvas.h"
#include "lost/Quad.h"
#include "lost/Application.h"
#include "lost/ResourceManager.h"
#include "lost/Camera2D.h"
#include "lost/HybridIndexBuffer.h"
#include "guiro/RenderContext.h"

extern lost::Application* _appInstance;

namespace lost
{
RenderSystem::RenderSystem()
{
  rc = new RenderContext(_appInstance->glContext);
  
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
  canvasQuad->material->shader = _appInstance->resourceManager->shader("resources/glsl/texture");
  canvasQuad->material->blendPremultiplied();
  uicam.reset(new Camera2D(Rect(0,0,newSize)));
}
  
void RenderSystem::draw(Context* glContext, const ViewPtr& rootView)
{
  draw(glContext, rootView->layer);
  glContext->bindDefaultFramebuffer();
  glContext->camera(uicam);
  glContext->draw(canvasQuad);
}

void RenderSystem::draw(Context* glContext, const LayerPtr& layer)
{
  canvas->drawToCanvas([this, layer](){
    rc->drawSolidRect(layer->rect, layer->backgroundColor);
  });
}

}

