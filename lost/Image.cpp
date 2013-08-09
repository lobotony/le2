#include "lost/Image.h"

namespace lost
{

Image::Image(const TexturePtr& tex)
{
  texture = tex;
  size = Vec2(texture->dataWidth, texture->dataHeight);
  originalPixelCoords = Rect(0,0,size.width, size.height);
  resizeMode = ImageResizeModeStretch;
  orientation = ImageOrientationUp;
  
  t = l = b = r = 0.0f; // reset ninepatch caps
  updateTextureCoords();
}

Image::~Image()
{
}

void Image::updateTextureCoords()
{
  bl = Vec2(0,0);
  br = Vec2(1,0);
  tr = Vec2(1,1);
  tl = Vec2(0,1);
}

}
