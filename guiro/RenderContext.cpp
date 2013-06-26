#include "guiro/RenderContext.h"
#include "lost/Context.h"
#include "lost/HybridIndexBuffer.h"
#include "lost/Mesh.h"
#include "lost/Application.h"
#include "lost/ResourceManager.h"
#include "lost/TextRender.h"
#include "lost/TextMesh.h"
#include "lost/Bitmap.h"

namespace lost
{

#pragma mark - de/construction -

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
  bgquad->material->blendPremultiplied();
  
  textMesh.reset(new TextMesh);
  textMesh->material->shader = textureShader;
  textMesh->material->color = whiteColor;
  
  _flipX = false;
  _flipY = false;
  updateTexCoords();
}

#pragma mark - resource management -

string RenderContext::quarterDiscPath(u16 radius)
{
  StringStream ss;
  ss << "qdisc-"<<radius;
  return ss.str();
}

string RenderContext::quarterRingPath(u16 radius, u16 thickness)
{
  StringStream ss;
  ss << "qring-"<<radius<<"-"<<thickness;
  return ss.str();
}

TexturePtr RenderContext::quarterDisc(u16 radius)
{
  TexturePtr result;
  
  string path = quarterDiscPath(radius);
  if(!Application::instance()->resourceManager->hasTexture(path))
  {
    BitmapPtr bitmap(new Bitmap(radius, radius, GL_RGBA));
    bitmap->disc(radius-1, radius-1, radius);
    bitmap->premultiplyAlpha();
    result.reset(new Texture(bitmap));
    Application::instance()->resourceManager->texture(path, result);
    bitmap->write("/Users/tony/"+path+".tga");
  }
  else
  {
    result = Application::instance()->resourceManager->texture(path);
  }
  
  return result;
}

TexturePtr RenderContext::quarterRing(u16 radius, u16 thickness)
{
  TexturePtr result;
  
  string path = quarterRingPath(radius, thickness);
  
  if(!Application::instance()->resourceManager->hasTexture(path))
  {
    BitmapPtr bitmap(new Bitmap(radius, radius, GL_RGBA));
    bitmap->ring(radius-1, radius-1, radius, thickness);
    bitmap->premultiplyAlpha();
    result.reset(new Texture(bitmap));
    Application::instance()->resourceManager->texture(path, result);
  }
  else
  {
    result = Application::instance()->resourceManager->texture(path);
  }
  
  return result;
}

#pragma mark - tex coord updates for image flipping - 

void RenderContext::updateTexCoords()
{
  if(!_flipX && !_flipY)
  {
    bgquad->set(0,UT_texcoord0, Vec2(0,0));
    bgquad->set(1,UT_texcoord0, Vec2(1,0));
    bgquad->set(2,UT_texcoord0, Vec2(1,1));
    bgquad->set(3,UT_texcoord0, Vec2(0,1));
  }
  else if(_flipX && !_flipY)
  {
    bgquad->set(0,UT_texcoord0, Vec2(1,0));
    bgquad->set(1,UT_texcoord0, Vec2(0,0));
    bgquad->set(2,UT_texcoord0, Vec2(0,1));
    bgquad->set(3,UT_texcoord0, Vec2(1,1));
  }
  else if(!_flipX && _flipY)
  {
    bgquad->set(0,UT_texcoord0, Vec2(0,1));
    bgquad->set(1,UT_texcoord0, Vec2(1,1));
    bgquad->set(2,UT_texcoord0, Vec2(1,0));
    bgquad->set(3,UT_texcoord0, Vec2(0,0));
  }
  else if(_flipX && _flipY)
  {
    bgquad->set(0,UT_texcoord0, Vec2(1,1));
    bgquad->set(1,UT_texcoord0, Vec2(0,1));
    bgquad->set(2,UT_texcoord0, Vec2(0,0));
    bgquad->set(3,UT_texcoord0, Vec2(1,0));
  }
}

void RenderContext::updateTexCoords(bool flipX, bool flipY)
{
  if((_flipX != flipX) || (_flipY != flipY))
  {
    _flipX = flipX;
    _flipY = flipY;
    updateTexCoords();
  }
}


#pragma mark - drawing -

void RenderContext::drawSolidRect(const Rect& rect, const Color& col)
{
  bgquad->transform = Matrix::translate(Vec3(rect.x, rect.y, 0)) * Matrix::scale(Vec3(rect.width, rect.height, 1));
  bgquad->material->color = col.premultiplied();
  bgquad->material->shader = colorShader;
  bgquad->material->blendPremultiplied();
  glContext->draw(bgquad);
}

void RenderContext::drawTexturedRect(const Rect& rect, const TexturePtr& tex, const Color& col, bool flipX, bool flipY)
{
  updateTexCoords(flipX, flipY);
  bgquad->transform = Matrix::translate(Vec3(rect.x, rect.y, 0)) * Matrix::scale(Vec3(rect.width, rect.height, 1));
  bgquad->material->color = col.premultiplied();
  bgquad->material->shader = textureShader;
  bgquad->material->limitTextures(1);
  bgquad->material->setTexture(0, tex);
  bgquad->material->blendPremultiplied();
  glContext->draw(bgquad);
}

void RenderContext::drawText(const string& text, const FontPtr& font, const Color& col, const Vec2& pos, int alignment)
{
  render(text, font, textMesh, true, alignment);
  textMesh->transform = Matrix::translate(Vec3(pos.x, pos.y, 0));
  textMesh->material->color = col.premultiplied();
  textMesh->material->blendPremultiplied();
  glContext->draw(textMesh);
}

void RenderContext::drawRoundRect(const Rect& rect, u16 r, const Color& col)
{
  TexturePtr tex = quarterDisc(r);
  drawRR(rect, r, tex, col);
}

void RenderContext::drawRoundRectFrame(const Rect& rect, u16 radius, u16 thickness, const Color& col)
{
  TexturePtr tex = quarterRing(radius, thickness);
  drawRR(rect, radius, tex, col);
}

void RenderContext::drawRR(const Rect& rect, u16 r, const TexturePtr& tex, const Color& col)
{
  // round corners
  Rect bl(rect.x, rect.y, r, r);
  Rect br(rect.x+rect.width-r, rect.y, r, r);
  Rect tr(rect.x+rect.width-r, rect.y+rect.height-r, r, r);
  Rect tl(rect.x, rect.y+rect.height-r, r, r);
  
  // 3 horizontal in-between slices
  Rect top(rect.x+r, rect.y+rect.height-r, rect.width-2*r, r);
  Rect mid(rect.x, rect.y+r, rect.width, rect.height-2*r);
  Rect bot(rect.x+r, rect.y, rect.width-2*r, r);
  
  drawTexturedRect(bl, tex, col, false, false);
  drawTexturedRect(br, tex, col, true, false);
  drawTexturedRect(tr, tex, col,true,true);
  drawTexturedRect(tl, tex, col,false, true);
  
  drawSolidRect(top, col);
  drawSolidRect(mid, col);
  drawSolidRect(bot, col);
}

}


