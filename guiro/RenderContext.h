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
  void drawTexturedRect(const Rect& rect, const TexturePtr& tex, const Color& col, bool flipX=false, bool flipY=false);
  void drawText(const string& text, const FontPtr& font, const Color& col, const Vec2& pos, int alignment);
  void drawRoundRect(const Rect& rect, u16 radius, const Color& col);
  void drawRoundRectFrame(const Rect& rect, u16 radius, u16 thickness, const Color& col);
  
  string quarterDiscPath(u16 radius);
  string quarterRingPath(u16 radius, u16 thickness);
  
  TexturePtr quarterDisc(u16 radius);
  TexturePtr quarterRing(u16 radius, u16 thickness);
  
  ShaderProgramPtr colorShader;
  ShaderProgramPtr textureShader;
  
  Context* glContext;
  MeshPtr bgquad;
  TextMeshPtr textMesh;
  
private:
  bool _flipX;
  bool _flipY;
  void updateTexCoords(); // always updates texcoords for current flip settings
  void updateTexCoords(bool flipX, bool flipY); // checks if new flags are different and triggers optional update
  void drawRR(const Rect& rect, u16 r, const TexturePtr& tex, const Color& col);
};
}

#endif

