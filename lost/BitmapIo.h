#ifndef LOST_BITMAP_IO_H
#define LOST_BITMAP_IO_H

#include "lost/Bitmap.h"

namespace lost
{

struct BitmapRawHeader
{
  u16 width;
  u16 height;
  u8  bytesPerPixel;
  u8  premultipliedAlpha;
};

// filepath must point to file containing decompressed bitmap data, prefixed with a BitmapRawHeader
// discard the pointer to unmap the file again.
BitmapPtr loadMapped(const string& filepath);

// writes the given bitmap into the file pointer to in filpath, prefixing it with a BitmapRawHeader, derived from Bitmap
void writeRawToPath(const BitmapPtr& bmp, const string& filepath);
}

#endif