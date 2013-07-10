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

#ifndef LOST_FONT_H
#define LOST_FONT_H

namespace lost
{
struct Font
{
  Font() {};
  virtual ~Font() {};
  
  virtual bool      hasKerning() = 0;
  virtual GlyphPtr  glyph(uint32_t utf32character) = 0;
  virtual float     kerningOffset(uint32_t previousChar, uint32_t currentChar) = 0;
  virtual float     characterAdvance(uint32_t previousChar, uint32_t currentChar) = 0; // returns advance (if glyph available)+kerningOffset (if kerning available), 0 otherwise. Use 0 if there is no prevchar

  uint32_t    size;
  TexturePtr  atlas;
  Vec2        atlasSize;
  float       lineHeight;
  float       ascender;
  float       descender;
};
}

#endif
