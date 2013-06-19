#include "lost/Canvas.h"

#include "lost/Application.h"
#include "lost/Camera2D.h"
#include "lost/FrameBuffer.h"
#include "lost/Context.h"
#include "lost/Quad.h"
#include "lost/ResourceManager.h"

extern lost::Application* _appInstance;

namespace lost
{
  
  Canvas::Canvas(const Vec2& sz)
  {
    size = sz;
    cam = Camera2D::create(Rect(0,0,size.width,size.height));

    framebuffer = FrameBuffer::create(size, GL_RGBA);
    framebuffer->check();
    _appInstance->glContext->bindDefaultFramebuffer();

    clearColor = Color(0,0,0,0);
  }

  void Canvas::drawToCanvas(const std::function<void(void)>& sceneFunc)
  {
    framebuffer->bind();
    _appInstance->glContext->clearColor(clearColor);
    _appInstance->glContext->camera(cam);
    _appInstance->glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

    sceneFunc();
  }
  
  TexturePtr Canvas::texture()
  {
    return framebuffer->texture(0);
  }

}