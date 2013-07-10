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

#ifndef LOST_BITMAPPACKER_H
#define LOST_BITMAPPACKER_H

#include "lost/RectPacker.h"

namespace lost
{
struct BitmapPacker
{
  struct Result
  {

    BitmapPtr            packedBitmap; // the resulting bitmap
    vector<Rect>   rects;        // the resulting rects. Size of this vector is number of bitmaps that fit into the target area
    vector<int32_t>      bitmapIds;    // the initial index of the bitmap in the incoming data
    vector<bool>         rotated;      // true if the bitmap was rotated

    void clear();
  };

  BitmapPacker();
  virtual ~BitmapPacker();

  /** Tries to pack as many bitmaps as possible into a new bitmap with the given size.
   *
   * @param outResult   receives the result of the packing
   * @param targetSize  size of the bitmap that contains all the provided bitmaps.
   * @param bitmaps     all bitmaps to pack.
   * @param format      format of the pixel components of the resulting bitmap.
   * @param rotate      set this to true if you want to allow rotation of the packed bitmaps
   */
  void pack(BitmapPacker::Result& outResult,
            const Vec2& targetSize,
            vector<BitmapPtr> bitmaps,
            GLenum format = GL_RGBA,
            bool rotate = false,  /* won't do anything for now, so you can leave it at false */
            bool sort = true); // use sorting for best packing density

private:
  void buildRectsFromBitmaps(vector<Rect>& outRects,
                             vector<BitmapPtr> inBitmaps);

  RectPacker rectPacker;
};
}

#endif
