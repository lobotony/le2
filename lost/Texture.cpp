#include "lost/Texture.h"
#include "lost/Data.h"
#include "lost/Bitmap.h"

namespace lost
{

Texture::Params::Params()
{
  level = 0;
  internalFormat = GL_RGBA;
  border = 0;
  format = GL_RGBA;
  type = GL_UNSIGNED_BYTE;
  wrapS = GL_CLAMP_TO_EDGE;
  wrapT = GL_CLAMP_TO_EDGE;
  minFilter = GL_NEAREST;
  magFilter = GL_NEAREST;
  sizeHint = SIZE_DONT_CARE;
}

void Texture::create()
{
  glGenTextures(1, &texture);GLASSERT;
  neverBeenBound = true;
}


Texture::Texture()
{
  create();
}

Texture::Texture(const Vec2& inSize, const Params& inParams)
{
  create();
  bind();
  init(inSize, inParams);
}

Texture::Texture(const DataPtr& inData,  const Params& inParams)
{
  create();
  bind();
  init(inData, inParams);
}

Texture::Texture(const BitmapPtr& inBitmap, const Params& inParams)
{
  create();
  bind();
  init(inBitmap, inParams);
}

Texture::~Texture()
{
  destroy();
}

void Texture::destroy()
{
  glDeleteTextures(1, &texture);GLDEBUG;
}

void Texture::bind() 
{
  glBindTexture(GL_TEXTURE_2D, texture);GLASSERT;
  neverBeenBound = false;
}

void Texture::init(const DataPtr& inData,  const Params& inParams)
{
  BitmapPtr bmp(new Bitmap(inData));
  init(bmp, inParams);
}

void Texture::init(const Vec2& inSize, const Texture::Params& inParams)
{
  bind();
  
  uint32_t texwidth = (uint32_t)inSize.width;
  uint32_t texheight = (uint32_t)inSize.height;
  Texture::SizeHint sizeHint = inParams.sizeHint;
  // if sizehint is dontcare we try to choose non-power-of-two, unless the platform doesn't allow it
  if(sizeHint == Texture::SIZE_DONT_CARE)
  {
/*#if TARGET_OPENGL
      sizeHint = Texture::SIZE_ORIGINAL;
#else
      sizeHint = Texture::SIZE_POWER_OF_TWO;
#endif*/
      sizeHint = Texture::SIZE_ORIGINAL;

  }

  if(sizeHint == Texture::SIZE_POWER_OF_TWO)
  {
    texwidth = nextPowerOf2(inSize.width);
    texheight = nextPowerOf2(inSize.height);
//    DOUT("creating power-of-two texture "<<texwidth<<" "<<texheight);
  }
  else
  {
//    DOUT("creating texture of size "<<texwidth<<" "<<texheight);
  }

  // create an empty texture object, i.e. without data, to setup the desired size
  init(inParams.level,
        inParams.internalFormat,
        texwidth,
        texheight,
        inParams.border ? 1 : 0,
        inParams.format,
        inParams.type,
        0);

  // memorize texture and raw data sizes for texture coordinate calculations
  width = texwidth;
  height = texheight;
  dataWidth = (uint32_t)inSize.width;
  dataHeight = (uint32_t)inSize.height;
}

void Texture::init(const BitmapPtr& inBitmap, const Texture::Params& inParams)
{
  bind();
  Texture::Params bitmapParams(inParams);
  bitmapParams.internalFormat = bitmapParams.format = inBitmap->format; // FIXME: is this correct?

  // creates texture with correct size according to size hint
  init(Vec2((float)inBitmap->width, (float)inBitmap->height), bitmapParams);

  // then use texsubimage to upload the actual data. Strictly speaking, we only need this in the
  // case of power-of-two textures, but always creating textures like this saves us one branch
  // now we copy the actual data to the previously allocated texture
  glTexSubImage2D(GL_TEXTURE_2D,
                  inParams.level,
                  0,
                  0,
                  inBitmap->width,
                  inBitmap->height,
                  bitmapParams.format,
                  inParams.type,
                  inBitmap->data);GLASSERT;

  // set wrapping and filters
  wrapS(bitmapParams.wrapS);
  wrapT(bitmapParams.wrapT);
  minFilter(bitmapParams.minFilter);
  magFilter(bitmapParams.magFilter);

  dataWidth = inBitmap->width;
  dataHeight = inBitmap->height;

}

void Texture::init(const vector<BitmapPtr>& inBitmaps, const Params& inParams)
{
  bind();  
  Texture::Params bitmapParams(inParams);
  bitmapParams.level = 0;
  bitmapParams.internalFormat = bitmapParams.format = inBitmaps[0]->format; // FIXME: is this correct?

  for(uint32_t i=0; i<inBitmaps.size(); ++i)
  {
    glTexImage2D(GL_TEXTURE_2D,
             bitmapParams.level,
             bitmapParams.internalFormat,
             inBitmaps[i]->width,
             inBitmaps[i]->height,
             bitmapParams.border,
             bitmapParams.format,
             bitmapParams.type,
             inBitmaps[i]->data);GLASSERT;
    ++bitmapParams.level;
  }
  this->internalFormat = bitmapParams.internalFormat;
  wrapS(bitmapParams.wrapS);
  wrapT(bitmapParams.wrapT);
  minFilter(bitmapParams.minFilter);
  magFilter(bitmapParams.magFilter);

  // size hint is ignored for now, size of first texture is used, assuming power-of-two and complete usage of area
  width = dataWidth = inBitmaps[0]->width;
  height = dataHeight = inBitmaps[0]->height;  
}

void Texture::subImage(const Vec2& targetPos, const BitmapPtr& inBitmap)
{
  glTexSubImage2D(GL_TEXTURE_2D,
                  0,
                  (GLint)targetPos.x,
                  (GLint)targetPos.y,
                  inBitmap->width,
                  inBitmap->height,
                  inBitmap->format,
                  GL_UNSIGNED_BYTE,
                  inBitmap->data);GLASSERT;
}

void Texture::init(GLint level, // mipmap level
           GLenum inInternalFormat, // number of color components
           GLsizei width,
           GLsizei height,
           GLint border, // must be 0 or 1
           GLenum format, // composition of each element in pixels
           GLenum type, // numerical type of provided pixel data
           const GLvoid* data) // pointer to the data or 0 if you only want to reserve data for later usage
{
    bind();
    glTexImage2D(GL_TEXTURE_2D,
                 level,
                 inInternalFormat,
                 width,
                 height,
                 border,
                 format,
                 type,
                 data);GLASSERT;
  this->internalFormat = inInternalFormat;
}

void Texture::wrap(GLint p)
{
  wrapS(p);
  wrapT(p);
}

void Texture::wrapS(GLint p)
{
  param(GL_TEXTURE_WRAP_S, p);
}

void Texture::wrapT(GLint p)
{
  param(GL_TEXTURE_WRAP_T, p);
}

void Texture::filter(GLint p)
{
  minFilter(p);
  magFilter(p);
}

void Texture::minFilter(GLint p)
{
  param(GL_TEXTURE_MIN_FILTER, p);
}

void Texture::magFilter(GLint p)
{
  param(GL_TEXTURE_MAG_FILTER, p);
}

void Texture::param(GLenum pname, GLint p)
{
  bind();
  glTexParameteri(GL_TEXTURE_2D, pname, p);GLASSERT;
}

}
