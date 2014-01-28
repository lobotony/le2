#pragma once

namespace lost
{

struct Canvas
{
  Canvas(const Vec2& sz);
  
  // drawing to a canvas leaves the framebuffer enabled, so you have to switch to the default buffer after calling drawToCanvas()
  virtual void drawToCanvas(const std::function<void(void)>& sceneFunc);
  
  TexturePtr texture();
  
  Vec2 size;
  CameraPtr cam;
  FrameBufferPtr framebuffer;
  Color clearColor;
};

}


