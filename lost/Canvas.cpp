#include "lost/Canvas.h"

#include "lost/Engine.h"
#include "lost/Camera2D.h"
#include "lost/FrameBuffer.h"
#include "lost/Context.h"
#include "lost/Quad.h"
#include "lost/ResourceManager.h"

extern lost::Engine* _engineInstance;

namespace lost
{
  
  Canvas::Canvas(const Vec2& sz)
  {
    size = sz;
    cam = Camera2D::create(Rect(0,0,size.width,size.height));

    framebuffer = FrameBuffer::create(size, GL_RGBA);
    framebuffer->check();
    _engineInstance->glContext->bindDefaultFramebuffer();

    clearColor = Color(0,0,0,0);
  }

  void Canvas::drawToCanvas(const std::function<void(void)>& sceneFunc)
  {
    framebuffer->bind();
    _engineInstance->glContext->clearColor(clearColor);
    _engineInstance->glContext->camera(cam);
    _engineInstance->glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

    sceneFunc();
  }

}