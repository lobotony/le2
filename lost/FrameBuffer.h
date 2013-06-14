/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

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

#ifndef LOST_GL_FRAMEBUFFER_H
#define LOST_GL_FRAMEBUFFER_H

#include "lost/Texture.h"
#include "lost/RenderBuffer.h"

namespace lost
{
  struct FrameBuffer;
  typedef lost::shared_ptr<FrameBuffer> FrameBufferPtr;

  struct FrameBuffer
  {

    struct Attachment;
    typedef lost::shared_ptr<Attachment> AttachmentPtr;

    struct Attachment
    {

      enum UsageType
      {
        UT_texture      = 0,
        UT_renderBuffer = 1
      };

    private:
      void createTexture();
      void updateTexture();
      void createRenderBuffer();
      void updateRenderBuffer();

      void update();

    public:
      Vec2 size;
      GLenum     bitFormat;
      UsageType  usageType;

      TexturePtr      texture;      // for UsageType UT_texture
      RenderBufferPtr renderBuffer; // for UsageType UT_renderBuffer

      static AttachmentPtr create(const Vec2& size, GLenum bitFormat, UsageType usageType = UT_texture);
      static AttachmentPtr create(const TexturePtr& texture);            // for UsageType UT_texture
      static AttachmentPtr create(const RenderBufferPtr& renderBuffer);  // for UsageType UT_renderBuffer

      Attachment(const Vec2& size, GLenum bitFormat, UsageType usageType = UT_texture);
      Attachment(const TexturePtr& texture);            // for UsageType UT_texture
      Attachment(const RenderBufferPtr& renderBuffer);  // for UsageType UT_renderBuffer

      ~Attachment();

      void attach(GLenum target);

      void resize(const Vec2& size);
      void resetBitFormat(GLenum bitFormat);

      void bind();
      
    };
    
  private:
    GLuint buffer;

  public:
    Vec2 size;

    map<uint8_t, AttachmentPtr> colorBuffers;
    AttachmentPtr                           depthBuffer;
    AttachmentPtr                           stencilBuffer;

    static FrameBufferPtr create(const Vec2& size, GLenum colorBits = -1, GLenum depthBits = -1, GLenum stencilBits = -1);

    FrameBuffer(const Vec2& size, GLenum colorBits = -1, GLenum depthBits = -1, GLenum stencilBits = -1);
    ~FrameBuffer();

    void attachColorBuffer(uint8_t index, const TexturePtr& buffer);
    void attachColorBuffer(uint8_t index, const AttachmentPtr& buffer);
    void attachDepthBuffer(const TexturePtr& buffer);
    void attachDepthBuffer(const AttachmentPtr& buffer);
    void attachStencilBuffer(const TexturePtr& buffer);
    void attachStencilBuffer(const AttachmentPtr& buffer);
    
    GLenum status();
    bool isComplete();
    void check();

    TexturePtr texture(u32 idx) { return colorBuffers[idx]->texture; }
        
    void bind();

    static void setup();
  };
}

#endif
