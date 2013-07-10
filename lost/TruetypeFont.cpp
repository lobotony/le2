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
#define STB_TRUETYPE_IMPLEMENTATION
#include "lost/TruetypeFont.h"

#include "lost/BitmapPacker.h"
#include "lost/Texture.h"
#include "lost/Bitmap.h"
#include "lost/Glyph.h"
#include "lost/Data.h"

namespace lost
{

TruetypeFont::TruetypeFont(const DataPtr& inData,
                           uint32_t inSizeInPoints)
{
  const unsigned char* data = reinterpret_cast<unsigned char*>(inData->bytes.get());
  
  _data = inData;
  ASSERT(stbtt_InitFont(&_fontinfo, data, 0), "font init failed")
  size = inSizeInPoints;
  atlasSize.width = 256;
  atlasSize.height = 256;

  _vscale = stbtt_ScaleForPixelHeight(&_fontinfo, size);
  
  int ascent = 0;
  int descent = 0;
  int lineGap = 0;
  stbtt_GetFontVMetrics(&_fontinfo, &ascent, &descent, &lineGap);
  
  ascender = floorf(ascent*_vscale);
  descender = floorf(descent*_vscale);
  lineHeight = floorf((ascent - descent + lineGap)*_vscale);
//  DOUT("font ascender:"<<ascender<<" descender:"<<descender<<" lineHeight:"<<lineHeight);
  rebuildTextureAtlas(); 
}

TruetypeFont::~TruetypeFont()
{
}

bool TruetypeFont::renderGlyph(char32_t c)
{
  bool result = false;
  GlyphPtr glyph = char2glyph[c];
  if(!glyph) 
  {
    result = true;
    glyph.reset(new Glyph);
//    glyph->render(face, size, c);
    
    int advance, lsb;
    stbtt_GetCodepointHMetrics(&_fontinfo, c, &advance, &lsb);
    
    int ix0, iy0, ix1, iy1;
    stbtt_GetCodepointBitmapBox(&_fontinfo, c, _vscale, _vscale, &ix0, &iy0, &ix1, &iy1);
    int width = ix1-ix0;
    int height = iy1-iy0;
    unsigned char* bmpmem = (unsigned char*)malloc(width*height);
    stbtt_MakeCodepointBitmapSubpixel(&_fontinfo, bmpmem, width, height, width, _vscale, _vscale, .0, .0, c);
    
    BitmapPtr result(new Bitmap(width,
                                height,
                                GL_RGBA,
                                GL_ALPHA,
                                bmpmem));  
    result->flip();
    free(bmpmem);
    glyph->bitmap = result;
    glyph->advance = floorf(advance*_vscale);
    glyph->xoffset = floorf(lsb*_vscale);
    glyph->yoffset = -iy1;
    
    char2glyph[c] = glyph;
    glyphs.push_back(glyph);
  }
  return result;
}

void TruetypeFont::rebuildTextureAtlas()
{
  BitmapPacker::Result  packerResult;
  
  vector<BitmapPtr> characterBitmaps;
  vector<uint32_t> glyphIndex; // because we might not use all bitmaps, need to preserve the original indices of the glyphs
  vector<GlyphPtr>::iterator i;
  uint32_t counter = 0;
  for(i=glyphs.begin(); i!=glyphs.end(); ++i)
  {
    // only use bitmaps that have width and height != 0
    // spaces might be rendered as 0/0 bitmaps, but have a valid advance value
    // that we need to render the string correctly.
    BitmapPtr bmp = (*i)->bitmap;
    if((bmp->width != 0) && (bmp->height != 0))
    {
      characterBitmaps.push_back(bmp);
      glyphIndex.push_back(counter);
    }
    counter++;
  }
  
  BitmapPacker bitmapPacker;
  bitmapPacker.pack(packerResult,
                    atlasSize,
                    characterBitmaps,
                    GL_RGBA, // bitmaps rendered byte the renderGlyphToBitmap method are always RGBA
                    false, // don't rotate rectbecause we don't support it yet
                    false); // don't sort because we don't want to alter position of previously rendered glyphs in texture
  
  ASSERT(packerResult.rects.size() == characterBitmaps.size(), "couldn't pack all bitmaps in characterAtlas");
  
  // recreate atlas texture from current packed bitmap
  // glyphs that were rendered in a previous pass should end up in the same spot in the atlas since we forced the packer to omit sorting
  if(!atlas)
  {
    //    DOUT("--- CREATING FONT ATLAS");
    atlas.reset(new Texture(packerResult.packedBitmap));
  }
  else
  {
    //    DOUT("reinitialising bitmap");
    atlas->bind();
    atlas->init(packerResult.packedBitmap);
  }
  
  // tell the glyphs where they are in the atlas texture in tex coords
  // the rects are reordered in the result, but the bitmapids are preserved in the vector of the same name
  for(uint32_t j=0; j<packerResult.rects.size(); ++j)
  {
    glyphs[glyphIndex[packerResult.bitmapIds[j]]]->rect = packerResult.rects[j];
  }  
}

GlyphPtr TruetypeFont::glyph(uint32_t utf32character)
{
  GlyphPtr result = char2glyph[utf32character];
  if(!result)
  {
    if(this->renderGlyph(utf32character))
    {
      rebuildTextureAtlas();
    }
    result = char2glyph[utf32character];
    if(result && result->bitmap && (result->bitmap->width > 0) && (result->bitmap->height > 0))
    {
      result->drawable = true;
    }
    else
    {
      if(result)
        result->drawable = false;
    }
  }
  return result;
}

float TruetypeFont::kerningOffset(uint32_t previousChar, uint32_t currentChar)
{
  float result = 0.0f;
  if(_fontinfo.kern) {
    result = stbtt_GetCodepointKernAdvance(&_fontinfo, previousChar, currentChar);
    result *= _vscale;
  }
  return result;
}

bool TruetypeFont::hasKerning()
{
  return _fontinfo.kern;
}

float TruetypeFont::characterAdvance(uint32_t previousChar, uint32_t currentChar)
{
  float result = 0.0f;
  GlyphPtr g = this->glyph(currentChar);
  if(g)
  {
    result = g->advance + kerningOffset(previousChar, currentChar);
  }
  return result;
}

}
