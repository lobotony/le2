#include "lost/DrawContext.h"
#include "lost/Context.h"
#include "lost/HybridIndexBuffer.h"
#include "lost/Mesh.h"
#include "lost/Application.h"
#include "lost/ResourceManager.h"
#include "lost/TextRender.h"
#include "lost/TextMesh.h"
#include "lost/Bitmap.h"
#include "lost/NinePatch.h"

namespace lost
{

#pragma mark - de/construction -

DrawContext::DrawContext(Context* ctx)
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
  
  ninePatch.reset(new NinePatch);
  ninePatch->flip = false;
  ninePatch->material->shader = textureShader;
  ninePatch->material->blendPremultiplied();
}

#pragma mark - resource management -

string DrawContext::discPath(u16 radius)
{
  StringStream ss;
  ss << "disc-"<<radius;
  return ss.str();
}

string DrawContext::ringPath(u16 radius, u16 thickness)
{
  StringStream ss;
  ss << "ring-"<<radius<<"-"<<thickness;
  return ss.str();
}

TexturePtr DrawContext::disc(u16 radius)
{
  TexturePtr result;
  
  string path = discPath(radius);
  if(!Application::instance()->resourceManager->hasTexture(path))
  {
    BitmapPtr bitmap(new Bitmap(2*radius, 2*radius, GL_RGBA));
    bitmap->disc(radius-.5, radius-.5, radius+.5);
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

TexturePtr DrawContext::ring(u16 radius, u16 thickness)
{
  TexturePtr result;
  
  string path = ringPath(radius, thickness);
  
  if(!Application::instance()->resourceManager->hasTexture(path))
  {
    BitmapPtr bitmap(new Bitmap(2*radius, 2*radius, GL_RGBA));
    bitmap->ring(radius-.5, radius-.5, radius, thickness);
    bitmap->premultiplyAlpha();
    bitmap->write("/Users/tony/"+path+".tga");
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

void DrawContext::updateTexCoords()
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

void DrawContext::updateTexCoords(bool flipX, bool flipY)
{
  if((_flipX != flipX) || (_flipY != flipY))
  {
    _flipX = flipX;
    _flipY = flipY;
    updateTexCoords();
  }
}


#pragma mark - drawing -

void DrawContext::drawSolidRect(const Rect& rect, const Color& col)
{
  bgquad->transform = Matrix::translate(Vec3(rect.x, rect.y, 0)) * Matrix::scale(Vec3(rect.width, rect.height, 1));
  bgquad->material->color = col.premultiplied();
  bgquad->material->shader = colorShader;
  bgquad->material->blendPremultiplied();
  glContext->draw(bgquad);
}

void DrawContext::drawTexturedRect(const Rect& rect, const TexturePtr& tex, const Color& col, bool flipX, bool flipY)
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

void DrawContext::drawText(const string& text, const FontPtr& font, const Color& col, const Vec2& pos, int alignment)
{
  render(text, font, textMesh, true, alignment);
  textMesh->transform = Matrix::translate(Vec3(pos.x, pos.y, 0));
  textMesh->material->color = col.premultiplied();
  textMesh->material->blendPremultiplied();
  glContext->draw(textMesh);
}

void DrawContext::drawRoundRect(const Rect& rect, u16 r, const Color& col)
{
  TexturePtr tex = disc(r);
  drawRR(rect, r, tex, col);
}

void DrawContext::drawRoundRectFrame(const Rect& rect, u16 radius, u16 thickness, const Color& col)
{
  TexturePtr tex = ring(radius, thickness);
  drawRR(rect, radius, tex, col);
}

void DrawContext::drawRR(const Rect& rect, u16 r, const TexturePtr& tex, const Color& col)
{
  ninePatch->update(tex, rect.size(), r, r, r, r);
  ninePatch->material->color = col.premultiplied();
  ninePatch->transform = Matrix::translate(Vec3(rect.x, rect.y, 0));
  glContext->draw(ninePatch);
}

}


