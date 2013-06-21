#include "guiro/RenderContext.h"
#include "lost/Context.h"
#include "lost/HybridIndexBuffer.h"
#include "lost/Mesh.h"
#include "lost/Application.h"
#include "lost/ResourceManager.h"

namespace lost
{

RenderContext::RenderContext(Context* ctx)
{
  glContext = ctx;
  // load some common shaders
  colorShader = Application::instance()->resourceManager->shader("resources/glsl/color");
  textureShader = Application::instance()->resourceManager->shader("resources/glsl/texture");
  
  // create buffers for efficient quad drawing. Vertex and index buffers are reused as often as possible
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
  
  bgquad->material->shader = colorShader;
  bgquad->material->color = whiteColor;
}

void RenderContext::drawSolidRect(const Rect& rect, const Color& col)
{
  bgquad->transform = Matrix::translate(Vec3(rect.x, rect.y, 0)) * Matrix::scale(Vec3(rect.width, rect.height, 1));
  bgquad->material->color = col;
  bgquad->material->shader = colorShader;
  glContext->draw(bgquad);
}

void RenderContext::drawTexturedRect(const Rect& rect, const TexturePtr& tex, const Color& col)
{
  bgquad->transform = Matrix::translate(Vec3(rect.x, rect.y, 0)) * Matrix::scale(Vec3(rect.width, rect.height, 1));
  bgquad->material->color = col;
  bgquad->material->shader = textureShader;
  bgquad->material->limitTextures(1);
  bgquad->material->setTexture(0, tex);
  glContext->draw(bgquad);
}

}

