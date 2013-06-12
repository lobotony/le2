#include "guiro/RenderSystem.h"
#include "lost/Canvas.h"
#include "lost/Quad.h"
#include "lost/Engine.h"
#include "lost/ResourceManager.h"
#include "lost/Camera2D.h"
#include "lost/HybridIndexBuffer.h"

extern lost::Engine* _engineInstance;

namespace lost
{
RenderSystem::RenderSystem()
{
  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  layout.add(ET_vec2_f32, UT_texcoord0);
  bgquad = Mesh::create(layout, ET_u16);
  bgquad->resetSize(4, 6);
  bgquad->indexBuffer->drawMode = GL_TRIANGLES;
  
  bgquad->set(0, UT_position, Vec2(0,0));
  bgquad->set(1, UT_position, Vec2(1,0));
  bgquad->set(2, UT_position, Vec2(1,1));
  bgquad->set(3, UT_position, Vec2(0,1));
  
  bgquad->set(0,UT_texcoord0, Vec2(0,0));
  bgquad->set(1,UT_texcoord0, Vec2(1,0));
  bgquad->set(2,UT_texcoord0, Vec2(1,1));
  bgquad->set(3,UT_texcoord0, Vec2(0,1));
  
  bgquad->set(0, UT_index, (u16)0);
  bgquad->set(1, UT_index, (u16)1);
  bgquad->set(2, UT_index, (u16)2);
  bgquad->set(3, UT_index, (u16)2);
  bgquad->set(4, UT_index, (u16)3);
  bgquad->set(5, UT_index, (u16)0);
  
  bgquad->material->shader = _engineInstance->resourceManager->shader("resources/glsl/color");
  bgquad->material->color = whiteColor;
  
  windowResized(Vec2(0,0)); // init so draw resources are not NULL, FIXME: take a close look at startup equence and event flow to avoid this?
}

RenderSystem::~RenderSystem()
{
}
  
void RenderSystem::windowResized(const Vec2& newSize)
{
  DOUT("");
  canvas.reset(new Canvas(newSize));
  canvasQuad = Quad::create(canvas->texture());
  canvasQuad->material->shader = _engineInstance->resourceManager->shader("resources/glsl/texture");
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
  canvas->drawToCanvas([this, glContext, layer](){
    bgquad->transform = Matrix::scale(Vec3(layer->rect.width, layer->rect.height, 1));
    bgquad->material->color = layer->backgroundColor;
    bgquad->material->shader = _engineInstance->resourceManager->shader("resources/glsl/color");
    glContext->draw(bgquad);
  });
}

}

