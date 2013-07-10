/*
Copyright (c) 2012 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/Camera2D.h"

namespace lost
{
Camera2D::Camera2D(const Rect& inViewport)
: Camera(inViewport)
{
  mDepth = Vec2(-1.0, 1.0);
  float offset = .375f;
  mViewMatrix = Matrix::translate(Vec3(offset, offset, .0f));
}

Camera2DPtr Camera2D::create(const Rect& inViewport)
{
  return Camera2DPtr(new Camera2D(inViewport));
}

void Camera2D::update()
{
  if (needsUpdate)
  {
    mProjectionMatrix = Matrix::ortho(Rect(0, 0, mViewport.width, mViewport.height), mDepth);
    needsUpdate = false;        
  }
}
}
