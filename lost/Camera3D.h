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

#pragma once

#include "lost/Camera.h"

namespace lost
{
struct Camera3D : public Camera
{
private:
  Vec3 mPosition;
  Vec3 mDirection;
  Vec3 mRotation;
  float      mFovY;
  bool       mStickToTarget;

protected:
  void update();

public:
  // constructor
  Camera3D(const Rect& inViewport);
  static Camera3DPtr create(const Rect& inViewport);
  // getters
  float fovY();
  Vec3 position();
  Vec3 direction();
  Vec3 rotation();
  bool stickToTarget();
  Vec3 target();
  Vec3 up();

  // setters
  void plane(const Vec3& inPlane);
  void fovY(const float inFovY);
  void position(const Vec3& newPosition);
  void direction(const Vec3& newDirection);
  void stickToTarget(const bool newStickToTarget);
  void rotation(const Vec3& newRotation);
  void target(const Vec3& newTarget);

  // methods
  void move(const Vec3& deltaMove);
  void rotate(const Vec3& deltaRotate);
};
}

