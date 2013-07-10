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


#ifndef LOST_TRUETYPEFONT_H
#define LOST_TRUETYPEFONT_H

#include "lost/Font.h"
#include "stb_truetype.h"

namespace lost
{

struct TruetypeFont : public Font
{
  TruetypeFont(const DataPtr& inData,
              uint32_t inSizeInPoints);
  virtual ~TruetypeFont();
  
  GlyphPtr glyph(uint32_t utf32character);
  bool hasKerning();
  float kerningOffset(uint32_t previousChar, uint32_t currentChar);
  float characterAdvance(uint32_t previousChar, uint32_t currentChar); // returns advance (if glyph available)+kerningOffset (if kerning available), 0 otherwise. Use 0 if there is no prevchar
  
  float             _vscale;
  stbtt_fontinfo    _fontinfo;
  
  DataPtr _data;
  std::map<char32_t, GlyphPtr> char2glyph;
  std::vector<GlyphPtr> glyphs; // this list of glyphs contains the glyphs in the order they were rendered
  // this is important to preserve the ordering fo rth packing of the atlas
  
  
  /** checks if the caches already contain the glyph for the given character 
   *  and updates it if they don't.
   *  @return true if the glyph was rendered, false if it was cached and didn't need to be rendered again.
   */
  bool renderGlyph(char32_t c);
  void rebuildTextureAtlas(); // builds a new texture atlas from the current glyphs in the cache.
  
};

}

#endif

