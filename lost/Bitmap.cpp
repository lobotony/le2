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

#include "lost/Data.h"
#include "lost/Bitmap.h"
#define STBI_HEADER_FILE_ONLY
#include "thirdparty/stb_image.c"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "thirdparty/stb_image_write.h"
  
namespace lost
{

void Bitmap::reset()
{
  data = NULL;
  loaded = false;
  width = 0;
  height = 0;
  premultiplied = false;
  format = 0;
  mmapaddr = NULL;
  mmapsize = 0;
}

Bitmap::Bitmap()
{
  reset();
}

Bitmap::Bitmap(uint32_t inWidth,
       uint32_t inHeight,
       GLenum format)
{
  reset();
  init(inWidth, inHeight, format);
}

Bitmap::Bitmap(uint32_t inWidth,
               uint32_t inHeight,
               GLenum destComponents,
               GLenum srcComponents,
               uint8_t* data)
{
  reset();
  init(inWidth, inHeight, destComponents, srcComponents, data);
}

Bitmap::Bitmap(const DataPtr& inData)
{
  reset();
  init(inData);
}

Bitmap::~Bitmap()
{
  destroy();
}

void Bitmap::destroy()
{
  if(data && loaded)
  {
    stbi_image_free(data);
  }
  else if(data && !loaded && !mmapaddr)
  {
    delete [] data;
  }
  reset();
}

void Bitmap::init(uint32_t inWidth,
          uint32_t inHeight,
          GLenum inFormat)
{
  destroy();
  // create target memory
  uint32_t destBytesPerPixel = bytesPerPixelFromComponents(inFormat);
  uint32_t destSizeInBytes = destBytesPerPixel * inWidth * inHeight;
  data = new uint8_t[destSizeInBytes];
  loaded = false; // prevent stb_image from freeing

  width = inWidth;
  height = inHeight;
  format = inFormat;
}


void Bitmap::init(uint32_t inWidth,
                  uint32_t inHeight,
                  GLenum destComponents,
                  GLenum srcComponents,
                  uint8_t* srcData)
{
  // basic initialisation of target area, size and flags
  init(inWidth, inHeight, destComponents);
  uint32_t destBytesPerPixel = bytesPerPixelFromComponents(format);

  // setup stc values for loop
  uint32_t srcBytesPerPixel = bytesPerPixelFromComponents(srcComponents);
  uint8_t* destWriter = data;
  uint8_t* srcReader = srcData;
  uint32_t numPixels = inWidth*inHeight;

  // copy pixels
  for(uint32_t currentPixel=0; currentPixel<numPixels; ++currentPixel)
  {
    copyPixel(destWriter, destComponents, srcReader, srcComponents);
    destWriter+=destBytesPerPixel;
    srcReader+=srcBytesPerPixel;
  }
}

// FIXME: not endian safe, we need to fix this for big endian
// FIXME: not sure if this is even correct byte order for little endian
//
// BIG MESS ALERT!!!!!
//
// the whole conversion process from one component type to another should really be programmable, but I can't be bothered right now.
// this thing was really only meant to convert alpha to rgba. This was tweaked to work for the fonts, everything else is an
// untested bonus. 
//
void Bitmap::copyPixel(uint8_t* dest,
                GLenum destComponents,
                uint8_t* src,
                GLenum srcComponents)
{
  switch(destComponents)
  {
    case GL_RGBA:
      switch(srcComponents)
      {
        case GL_RGBA:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];dest[3]=src[3];break;
        case GL_RGB:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];dest[3]=255;break;
        case GL_ALPHA:dest[0]=src[0];dest[1]=src[0];dest[2]=src[0];dest[3]=src[0];break;
        default:ASSERT(false, "can't copy pixel from source with components: " << srcComponents);
      }
      break;
    case GL_RGB:
      switch(srcComponents)
      {
        case GL_RGBA:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];break;
        case GL_RGB:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];break;
        case GL_ALPHA:dest[0]=src[0];dest[1]=src[0];dest[2]=src[0];break;
        default:ASSERT(false, "can't copy pixel from source with components: " << srcComponents);
      }
      break;
    case GL_ALPHA:
      switch(srcComponents)
      {
        case GL_RGBA:dest[0]=src[3];break;
        case GL_RGB:dest[0]=255;break;
        case GL_ALPHA:dest[0]=src[0];break;
        default:ASSERT(false, "can't copy pixel from source with components: " << srcComponents);
      }
      break;
    default:ASSERT(false, "can't copy pixel to destination with components: " << destComponents);
  }
}

uint32_t Bitmap::bytesPerPixelFromComponents(GLenum components)
{
  uint32_t result = 0;
  switch(components)
  {
    case GL_ALPHA:result = 1;break;
    case GL_RGB:result = 3;break;
    case GL_RGBA:result = 4;break;
    default:ASSERT(false, "can't determine bytes per pixel from components: " << components);
  }
  return result;
}

void Bitmap::init(const DataPtr& inData)
{
  destroy();
//  DOUT("init image from memory: " << inFile->location);
  int bytesPerPixel, w, h;
  data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(inData->bytes.get()), (s32)inData->size, &w, &h, &bytesPerPixel, 0);
  width = w;
  height = h;
  ASSERT(data, "couldn't init image from memory: "<<std::string(stbi_failure_reason()));

  switch(bytesPerPixel)
  {
    case 3:format = GL_RGB;break;
    case 4:format = GL_RGBA;break;
    default:ASSERT(false, "couldn't init image, don't know what to do with bytesPerPixel: " << bytesPerPixel);
  }
  loaded = true;
}

void Bitmap::clear(const Color& inColor)
{
  uint32_t bpp = bytesPerPixelFromComponents(format);
  switch(bpp)
  {
    case 1:clearA(inColor);break;
    case 3:clearRGB(inColor);break;
    case 4:clearRGBA(inColor);break;
    default:ASSERT(false, "couldn't clear image with bpp: " << bpp);
  }
}

void Bitmap::clearA(const Color& inColor)
{
  uint8_t v = static_cast<uint8_t>(inColor.a()*255.0f);
  uint32_t numpixels = width * height;
  for(uint32_t i=0; i<numpixels; ++i)
    data[i] = v;
}

void Bitmap::clearRGB(const Color& inColor)
{
  uint32_t numpixels = width * height;
  uint32_t bpp = 3;
  uint8_t* d = data;
  uint8_t r = static_cast<uint8_t>(inColor.r()*255.0f);
  uint8_t g = static_cast<uint8_t>(inColor.g()*255.0f);
  uint8_t b = static_cast<uint8_t>(inColor.b()*255.0f);
  uint32_t bytesize = numpixels * bpp;

  for(uint32_t i=0; i<bytesize; i+=bpp)
  {
    d[i+0] = r;
    d[i+1] = g;
    d[i+2] = b;
  }
}

void Bitmap::clearRGBA(const Color& inColor)
{
  uint32_t numpixels = width * height;
  uint32_t bpp = 4;
  uint8_t* d = data;
  uint8_t r = static_cast<uint8_t>(inColor.r()*255.0f);
  uint8_t g = static_cast<uint8_t>(inColor.g()*255.0f);
  uint8_t b = static_cast<uint8_t>(inColor.b()*255.0f);
  uint8_t a = static_cast<uint8_t>(inColor.a()*255.0f);
  uint32_t bytesize = numpixels * bpp;

  for(uint32_t i=0; i<bytesize; i+=bpp)
  {
    d[i+0] = r;
    d[i+1] = g;
    d[i+2] = b;
    d[i+3] = a;
  }
}

void Bitmap::write(const string inFullPathname)
{
  if(!stbi_write_tga(inFullPathname.c_str(), width, height, bytesPerPixelFromComponents(format), data))
  {
    ASSERT(false, "screenshot save failed");
  }
}

void Bitmap::flip()
{
  unsigned long pixelSizeBytes = bytesPerPixelFromComponents(format);
  // flip vertically because OpenGL returns it the other way round
  unsigned long lineInBytes = width * pixelSizeBytes;
  unsigned long halfHeight = height / 2; // deliberately round down if height is odd
  unsigned char* dataBytes = data;
  for(unsigned long bottomLine=0; bottomLine<halfHeight; ++bottomLine)
  {
    unsigned long topLine = (height - 1) - bottomLine;
    for(unsigned long bi=0; bi<lineInBytes; ++bi)
    {
      unsigned long topLineByte = width*topLine*pixelSizeBytes+bi;
      unsigned long bottomLineByte = width*bottomLine*pixelSizeBytes+bi;
      unsigned char b = dataBytes[topLineByte];
      dataBytes[topLineByte] = dataBytes[bottomLineByte];
      dataBytes[bottomLineByte] = b;
    }
  }
}

uint8_t* Bitmap::pixelPointer(uint32_t x, uint32_t y)
{
  uint32_t bpp = bytesPerPixelFromComponents(format);
  uint8_t* target = data+((x+(y*width))*bpp);
  return target;
}


void Bitmap::pixel(uint32_t x, uint32_t y, const Color& inColor)
{
  assert((x<width) && (y<height));
  uint8_t* target = pixelPointer(x,y);
  switch(format)
  {
    case GL_ALPHA:target[0] =inColor.au8();break;
    case GL_RGB:
      target[0]=inColor.ru8();
      target[1]=inColor.gu8();
      target[2]=inColor.bu8();
      break;
    case GL_RGBA:
      target[0]=inColor.ru8();
      target[1]=inColor.gu8();
      target[2]=inColor.bu8();
      target[3]=inColor.au8();
      break;
    default:
      break;
  }
}

/** reads a pixel from the given coordinates and returns it as a Color. */
  Color Bitmap::pixel(uint32_t x, uint32_t y)
{
  uint8_t* target = pixelPointer(x,y);
  Color result;
  switch(format)
  {
    case GL_ALPHA:
      result.r(1);
      result.g(1);
      result.b(1);
      result.au8(target[0]);break;
    case GL_RGB:
      result.ru8(target[0]);
      result.gu8(target[1]);
      result.bu8(target[2]);
      result.a(1);
      break;
    case GL_RGBA:
      result.ru8(target[0]);
      result.gu8(target[1]);
      result.bu8(target[2]);
      result.au8(target[3]);
      break;
    default:
      break;
  }
  return result;
}


lost::shared_ptr<Bitmap> Bitmap::rotCW()
{
  shared_ptr<Bitmap> result(new Bitmap(height, width, GL_RGBA));
  for(uint32_t y=0; y<height; ++y)
  {
    for(uint32_t x=0; x<width; ++x)
    {
      result->pixel(y, x, pixel((width-1)-x,y));
    }
  }
  return result;
}

void Bitmap::hline(uint32_t y, uint32_t xl, uint32_t xr, const Color& inColor)
{
  for(uint32_t x=xl; x<xr; ++x)
  {
    pixel(x,y,inColor);
  }
}

void Bitmap::vline(uint32_t x, uint32_t yb, uint32_t yt, const Color& inColor)
{
  for(uint32_t y=yb; y<yt; ++y)
  {
    pixel(x,y,inColor);
  }
}

void Bitmap::draw(uint32_t x, uint32_t y, lost::shared_ptr<Bitmap> target)
{
  if((format != target->format) || (format != GL_RGBA)) // slow path
  {
    for(uint32_t cy=0; cy<height; ++cy)
    {
      for(uint32_t cx=0; cx<width; ++cx)
      {
        target->pixel(x+cx, y+cy, pixel(cx, cy));
      }
    }
  }
  else // fast path for GL_RGBA
  {
    u32* sourceDataPtr = (u32*)data;
    u32 sourceLineSize = width;
    
    u32* targetDataPtr = (u32*)target->data;
    u32 targetLineSize = target->width;
    
    for(u32 cy=0; cy<height; ++cy)
    {
      for(u32 cx=0; cx<width; ++cx)
      {
        u32* targetPixel = targetDataPtr + cx+x + ((y+cy)*targetLineSize);
        u32* sourcePixel = sourceDataPtr + cx + cy*sourceLineSize;
        *targetPixel = *sourcePixel;
      }
    }
  }
}

void Bitmap::drawRectOutline(const Rect& inRect, const Color& inColor)
{
  float minX = inRect.x;
  float minY = inRect.y;
  float maxX = inRect.maxX();
  float maxY = inRect.maxY();

  vline((uint32_t)minX, (uint32_t)minY, (uint32_t)maxY, inColor);
  vline((uint32_t)maxX, (uint32_t)minY, (uint32_t)maxY, inColor);
  hline((uint32_t)minY, (uint32_t)minX, (uint32_t)maxX, inColor);
  hline((uint32_t)maxY, (uint32_t)minX, (uint32_t)maxX, inColor);
}

void Bitmap::disc(float x, float y, float r)
{
  r = std::max(1.0f, r-1.0f);
  float w = (float)width;
  float h = (float)height;
  Color white(1,1,1,1);
  Color black(0,0,0,0);
  for(float cx=0; cx<w; cx+=1.0f)
  {
    for(float cy=0; cy<h; cy+=1.0f)
    {
      float dx = cx-x;
      float dy = cy-y;
      float cr = sqrtf(dx*dx+dy*dy);
      pixel((uint32_t)cx, (uint32_t)cy, (cr < r) ? white : black);
      float dr = cr-r;
      if(dr >= 0.0f && (dr < 1.0f))
      {
        float fract = 1.0f-(dr - floor(dr));
        pixel((uint32_t)cx, (uint32_t)cy, Color(1,1,1, fract));
      }
    }
  }
}

void Bitmap::ring(float x, float y, float r, float t)
{
  r = std::max(1.0f, r-1.0f);
  float w = (float)width;
  float h = (float)height;
  float lr = r - t;
  Color white(1,1,1,1);
  Color black(0,0,0,0);
  for(float cx=0; cx<w; cx+=1.0f)
  {
    for(float cy=0; cy<h; cy+=1.0f)
    {
      float dx = cx-x;
      float dy = cy-y;
      float cr = sqrtf(dx*dx+dy*dy);
      pixel((uint32_t)cx, (uint32_t)cy, ((cr < r) && (cr > lr)) ? white : black);

      // outer AA
      float dr = cr-r;
      if(dr >= 0.0f && (dr < 1.0f))
      {
        float fract = 1.0f-(dr - floor(dr));
        pixel((uint32_t)cx, (uint32_t)cy, Color(1,1,1,fract));
      }

      // inner AA
      dr = cr-lr;
      if(dr >= 0.0f && (dr < 1.0f))
      {
        float fract = (dr - floor(dr));
        pixel((uint32_t)cx, (uint32_t)cy, Color(1,1,1,fract));
      }
    }
  }
}

void Bitmap::filledRect(const Color& col, uint32_t posx, uint32_t posy, uint32_t sizew, uint32_t sizeh)
{
  uint32_t maxx = posx+sizew;
  uint32_t maxy = posy+sizeh;
  for(uint32_t x=posx; x<maxx; ++x)
  {
    for(uint32_t y=posy; y<maxy; ++y)
    {
      pixel(x,y,col);
    }
  }
}

void Bitmap::premultiplyAlpha()
{
  if(!premultiplied)
  {
    for(uint32_t x=0; x<width; ++x)
    {
      for(uint32_t y=0; y<height; ++y)
      {
        Color c = pixel(x,y);
        float a = c.fv[3];
        c.fv[0] = c.fv[0]*a;
        c.fv[1] = c.fv[1]*a;
        c.fv[2] = c.fv[2]*a;
        pixel(x,y,c);
      }
    }
    premultiplied = true;
  }
}

}
