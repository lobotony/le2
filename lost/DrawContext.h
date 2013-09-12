#ifndef LOST_DRAWCONTEXT_H
#define LOST_DRAWCONTEXT_H

namespace lost
{

struct Context;
struct TextBuffer;

/** Bundles common functions and resources for efficient 2D UI drawing.
 * Layers don't need to use this, but having this helps share some resources.
 */
struct DrawContext
{
  DrawContext(Context* ctx);
  
  void drawSolidRect(const Rect& rect, const Color& col);
  void drawTexturedRect(const Rect& rect, const TexturePtr& tex, const Color& col, bool flipX=false, bool flipY=false);
  void drawText(const string& text, const FontPtr& font, const Color& col, const Vec2& pos, int alignment);

  void drawText(const string& text,
                const FontPtr& font,
                const Color& col,
                const Rect& targetRect,
                const Vec2& offset,
                TextAlignment alignment,
                BreakMode breakmode);
  
  void drawRoundRect(const Rect& rect, u16 radius, const Color& col);
  void drawRoundRectFrame(const Rect& rect, u16 radius, u16 thickness, const Color& col);
  void drawRectFrame(const Rect& rect, u16 thickness, const Color& col);
  
  void drawImage(const ImagePtr& image, const Rect& rect, const Color& col);
  
  string discPath(u16 radius);
  string ringPath(u16 radius, u16 thickness);
  
  TexturePtr disc(u16 radius);
  TexturePtr ring(u16 radius, u16 thickness);
  
  ShaderProgramPtr colorShader;
  ShaderProgramPtr textureShader;
  
  Context* glContext;
  MeshPtr bgquad;
  TextMeshPtr textMesh;
  NinePatchPtr ninePatch;
  
private:
  TextBuffer* _textBuffer;
  bool _flipX;
  bool _flipY;
  void updateTexCoords(); // always updates texcoords for current flip settings
  void setTexCoords(const Vec2& bl, const Vec2& br, const Vec2& tr, const Vec2& tl); // always updates texcoords for current flip settings
  void updateTexCoords(bool flipX, bool flipY); // checks if new flags are different and triggers optional update
  void updateQuadTexCoordsFromImage(ImagePtr image);
  void drawRR(const Rect& rect, u16 r, const TexturePtr& tex, const Color& col);
  void drawImageStretched(const ImagePtr& image, const Rect& rect, const Color& col);
  void drawImageNinepatched(const ImagePtr& image, const Rect& rect, const Color& col);
  void drawImageTiled(const ImagePtr& image, const Rect& rect, const Color& col);
};
}

#endif

