#ifndef LOST_TEXTURE_H
#define LOST_TEXTURE_H

#include "lost/Vec2.h"

namespace lost
{
  /** a 2D Texture helper class.
   *
   */
  struct Texture 
  {
    enum SizeHint
    {
      SIZE_ORIGINAL = 0,
      SIZE_POWER_OF_TWO,
      SIZE_DONT_CARE,
      SIZE_NUM
    };
    
    struct Params
    {
      Params();
      
      GLint     level;          // mipmap level
      GLenum    internalFormat; // number of color components 
      GLint     border;         // 1 = with border, 0 = without
      GLenum    format;         // format of the incoming bitmap data: number and kind of components, sequence
      GLenum    type;           // type of the color components
      GLint     wrapS;          // default s wrapping mode that will be set after construction
      GLint     wrapT;          // default t wrapping mode that will be set after construction
      GLint     minFilter;      // default minification filter that will be set after construction
      GLint     magFilter;      // default magnification filter that will be set after construction
      SizeHint  sizeHint;       // size for constructing power of two textures depending on the capabilities of the platform
                                // with this flag, you can force (non) power of two textures or just leave it up
                                // to the Texture class to decide what's best
    };
  
    Texture();
    Texture(const Vec2& inSize, const Params& inParams = Params());
    Texture(const DataPtr& inData,  const Params& inParams = Params());
    Texture(const BitmapPtr& inBitmap, const Params& inParams = Params());      
    
    static TexturePtr create(const Vec2& inSize, const Params& inParams = Params()) { return TexturePtr(new Texture(inSize, inParams)); };
    static TexturePtr create(const DataPtr& inData,  const Params& inParams = Params()) { return TexturePtr(new Texture(inData, inParams)); };
    static TexturePtr create(const BitmapPtr& inBitmap, const Params& inParams = Params()) { return TexturePtr(new Texture(inBitmap, inParams)); };
    
    ~Texture();
    void destroy();

    void bind();
    
    void init(const DataPtr& inData,  const Params& inParams = Params());
    void init(const BitmapPtr& inBitmap, const Params& inParams = Params());
    void init(const Vec2& inSize, const Params& inParams = Params());
    void init(const vector<BitmapPtr>& inBitmaps, const Params& inParams = Params()); // assumes inBitmaps[0] is the largest, all others reductions, doesn't check for power of two, bitmaps must have same format
    
    void init(GLint level, // mipmap level
               GLenum internalformat, // number of color components
               GLsizei width,
               GLsizei height,
               GLint border, // must be 0 or 1 
               GLenum format, // composition of each element in pixels 
               GLenum type, // numerical type of provided pixel data
               const GLvoid* data); // pointer to the data or 0 if you only want to reserve data for later usage

    void subImage(const Vec2& targetPos, const BitmapPtr& bmp);

    void wrap(GLint p);
    void wrapS(GLint p);
    void wrapT(GLint p);

    void filter(GLint p);
    void minFilter(GLint p);
    void magFilter(GLint p);

    void param(GLenum pname, GLint p);

    GLuint          texture;
    GLenum          internalFormat;
    uint32_t width;
    uint32_t height;      
    uint32_t dataWidth; 
    uint32_t dataHeight;

    // this is required to work around a caching bug in gl::Context, because if a Texture with id 14 was destroyed and then immediately recreated, it MUST 
    // be bound or it will not be correctly used. neverBeenBound will be set to true in the constructor and then to false once the texture was bound 
    // for the first time.
    bool neverBeenBound; 
    
    inline uint32_t dataMaxY() { return dataHeight ? dataHeight-1 : dataHeight; }
    inline uint32_t dataMaxX() { return dataWidth ? dataWidth-1 : dataWidth; }

    // texture coordinate helpers
    // returns a normalised texture coordinate for the given pixel X coordinate in the range [0,1]
    // the texcoord is correctly offset so that the coord is centered on a texel for pixel perfect 2D drawing.
    inline float normalisedXCoord(uint32_t pixelXCoord)
    {
      float result = 0.0f;
      if(width > 0)
      {
        result = (((float)pixelXCoord))/((float)width);
      }
      
      return result;
    }

    // returns a normalised texture coordinate for the given pixel Y coordinate in the range [0,1]
    // the texcoord is correctly offset so that the coord is centered on a texel for pixel perfect 2D drawing.
    inline float normalisedYCoord(uint32_t pixelYCoord)
    {
      float result = 0.0f;
      if(height > 0)
      {
        result = (((float)pixelYCoord))/((float)height);
      }
      
      return result;
    }

    inline Vec2 normalisedCoord(const Vec2& pixelCoord)
    {
      return Vec2(normalisedXCoord((uint32_t)pixelCoord.x), normalisedYCoord((uint32_t)pixelCoord.y));
    }
    
    // returns pixel 0,0 as normalised textur coordianate 
    inline Vec2 bottomLeftTexCoord()
    {
      return normalisedCoord(Vec2(0,0));
    }
    
    // returns the top right bitmap pixel as normalised textur coordinate
    // this funcrtion does take into account that the ctua texture size can be larger than 
    // the uploaded data, so you can be sure to receive only the topright most bitmap pixel
    inline Vec2 topRightTexCoord()
    {
      uint32_t maxX = std::min(dataWidth, width);
      uint32_t maxY = std::min(dataHeight, height);
//        float fMaxX = maxX ? (float)(maxX-1) : (float)maxX;
//        float fMaxY = maxY ? (float)(maxY-1) : (float)maxY;
      return normalisedCoord(Vec2((float)maxX, (float)maxY));
    }
    
  private:
    Texture(const Texture&) {} // forbidden
    void create();
  };
}

#endif

