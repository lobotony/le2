#ifndef GUIRO_RENDERCONTEXT_H
#define GUIRO_RENDERCONTEXT_H

#include "lost/Rect.h"
#include "lost/Color.h"

namespace lost
{

struct Context;

/** Bundles common functions and resources for efficient 2D UI drawing.
 * Layers don't need to use this, but having this helps share some resources.
 */
struct RenderContext
{
  RenderContext(Context* ctx);
  
  void drawSolidRect(const Rect& rect, const Color& col);
  
  ShaderProgramPtr colorShader;
  
  Context* glContext;
  MeshPtr bgquad;  
};
}

#endif

