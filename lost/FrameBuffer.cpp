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

#include "lost/FrameBuffer.h"
#include "lost/lmath.h"

namespace lost
{

/**
 *  FrameBuffer::Attachment
 *
 *  holds a framebuffer attachment, that is: a texture or renderbuffer
 */

    FrameBuffer::AttachmentPtr FrameBuffer::Attachment::create(const Vec2& size, GLenum bitFormat, UsageType usageType)
    {
      return FrameBuffer::AttachmentPtr(new Attachment(size, bitFormat, usageType));
    }

    FrameBuffer::AttachmentPtr FrameBuffer::Attachment::create(const TexturePtr& texture)
    {
      return FrameBuffer::AttachmentPtr(new Attachment(texture));
    }

    FrameBuffer::AttachmentPtr FrameBuffer::Attachment::create(const RenderBufferPtr& renderBuffer)
    {
      return FrameBuffer::AttachmentPtr(new Attachment(renderBuffer));
    }

    FrameBuffer::Attachment::Attachment(const Vec2& size, GLenum bitFormat, UsageType usageType)
    {
      this->bitFormat = bitFormat;
      this->size = size;
      this->usageType = usageType;

      switch (usageType)
      {
        case UT_texture:
          createTexture();
          break;

        case UT_renderBuffer:
          createRenderBuffer();
          break;

        default:
          ASSERT(false, "invalid usage type: " << usageType);
      }
    }

    void FrameBuffer::Attachment::createTexture()
    {
      texture.reset(new Texture());
      updateTexture();
    }
    
    void FrameBuffer::Attachment::updateTexture()
    {
      texture->bind();
      Texture::Params textureParams;
      textureParams.internalFormat = bitFormat;
      switch (bitFormat)
      {
        case GL_DEPTH_COMPONENT16:
        case GL_DEPTH_COMPONENT24:
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE && !defined ANDROID
        case GL_DEPTH_COMPONENT32:
#endif        
          textureParams.format = GL_DEPTH_COMPONENT; // FIXME: this is probably the wrong constant, jst a try for ES
          textureParams.type = GL_FLOAT;
          break;
      }
      texture->init(size, textureParams);
      texture->filter(GL_NEAREST);
    }
    
    void FrameBuffer::Attachment::createRenderBuffer()
    {
      renderBuffer.reset(new RenderBuffer());
      updateRenderBuffer();
    }

    void FrameBuffer::Attachment::updateRenderBuffer()
    {
      renderBuffer->enable();
      renderBuffer->storage(bitFormat, (GLsizei)size.width, (GLsizei)size.height);
      renderBuffer->disable();
    }

    void FrameBuffer::Attachment::update()
    {
      switch (usageType)
      {
        case Attachment::UT_texture:
          updateTexture();
          break;
          
        case Attachment::UT_renderBuffer:
          updateRenderBuffer();
          break;
          
        default:
          ASSERT(false, "invalid usage type: " << usageType);
      }
    }

    FrameBuffer::Attachment::Attachment(const TexturePtr& texture)
    {
      bitFormat = texture->internalFormat;
      size = Vec2((float)texture->width, (float)texture->height);
      usageType = UT_texture;

      this->texture = texture;
      this->texture->bind();
      this->texture->filter(GL_NEAREST);
    }

    FrameBuffer::Attachment::Attachment(const RenderBufferPtr& renderBuffer)
    {
      bitFormat = renderBuffer->internalFormat();
      size = Vec2((float)renderBuffer->width(), (float)renderBuffer->height());
      usageType = UT_renderBuffer;
      
      this->renderBuffer = renderBuffer;
    }

    FrameBuffer::Attachment::~Attachment()
    {
      if (texture) texture.reset();
      if (renderBuffer) renderBuffer.reset();
    }

    void FrameBuffer::Attachment::resetBitFormat(GLenum bitFormat)
    {
      this->bitFormat = bitFormat;
      update();
    }

    void FrameBuffer::Attachment::attach(GLenum target)
    {
      switch (usageType)
      {
        case Attachment::UT_texture:
          texture->bind();
          glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, texture->texture, 0); GLASSERT;
          break;
        case Attachment::UT_renderBuffer:
          renderBuffer->enable();
          glFramebufferRenderbuffer(GL_FRAMEBUFFER, target, GL_RENDERBUFFER, renderBuffer->buffer); GLASSERT;
          renderBuffer->disable();
          break;
        default:
          ASSERT(false, "invalid usage type: " << usageType);
      }
    }

    void FrameBuffer::Attachment::detach(GLenum target)
    {
      switch (usageType)
      {
        case Attachment::UT_texture:
            glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, 0, 0); GLASSERT;
            break;
        case Attachment::UT_renderBuffer:
          glFramebufferRenderbuffer(GL_FRAMEBUFFER, target, GL_RENDERBUFFER, 0); GLASSERT;
          break;
        default:
          ASSERT(false, "invalid usage type: " << usageType);
      }
    }

    void FrameBuffer::Attachment::bind()
    {
      switch (usageType)
      {
        case Attachment::UT_texture:
          texture->bind();
          break;
          
        case Attachment::UT_renderBuffer:
          renderBuffer->enable();
          break;
          
        default:
          ASSERT(false, "invalid usage type: " << usageType);
      }
    }
    
    
/**
 *  FrameBuffer
 *
 *  the actual framebuffer implementation
 */

    FrameBufferPtr FrameBuffer::create(const Vec2& size, GLenum colorBits, GLenum depthBits, GLenum stencilBits)
    {
      return FrameBufferPtr(new FrameBuffer(size, colorBits, depthBits, stencilBits));
    }

    FrameBuffer::FrameBuffer(const Vec2& size, GLenum colorBits, GLenum depthBits, GLenum stencilBits)
    {
      DOUT("creating framebuffer with size "<<int(size.x)<<" "<<int(size.y));
      this->size = size;

      glGenFramebuffers(1, &buffer); GLASSERT;

      bind();

      if (colorBits != -1)
      {
        AttachmentPtr attachment = Attachment::create(size, colorBits, Attachment::UT_texture);
        attachColorBuffer(0, attachment);
      }
      if (depthBits != -1)
      {
        AttachmentPtr attachment = Attachment::create(size, depthBits, Attachment::UT_texture);
        attachDepthBuffer(attachment);
      }
      if (stencilBits != -1)
      {
        AttachmentPtr attachment = Attachment::create(size, stencilBits, Attachment::UT_renderBuffer);
        attachStencilBuffer(attachment);
      }

      // reset
      // FIXME: 
//      bindDefaultFramebuffer();
    }
    
    FrameBuffer::~FrameBuffer()
    {
      glDeleteFramebuffers(1, &buffer); GLASSERT;
    }

    void FrameBuffer::detachAll()
    {
      if(depthBuffer)
      {
        depthBuffer->detach(GL_DEPTH_ATTACHMENT);
      }
      if(stencilBuffer)
      {
        stencilBuffer->detach(GL_STENCIL_ATTACHMENT);
      }
      for(u32 i=0; i<colorBuffers.size();++i)
      {
        colorBuffers[i]->detach(GL_COLOR_ATTACHMENT0+i);
      }
    }

    void FrameBuffer::attachColorBuffer(uint8_t index, const TexturePtr& buffer)
    {
      attachColorBuffer(index, Attachment::create(buffer));
    }
    
    void FrameBuffer::attachColorBuffer(uint8_t index, const AttachmentPtr& buffer)
    {
      colorBuffers[index] = buffer;
      buffer->attach(GL_COLOR_ATTACHMENT0 + index);
    }
    
    void FrameBuffer::attachDepthBuffer(const TexturePtr& buffer)
    {
      attachDepthBuffer(Attachment::create(buffer));
    }
    
    void FrameBuffer::attachDepthBuffer(const AttachmentPtr& buffer)
    {
      depthBuffer = buffer;
      depthBuffer->attach(GL_DEPTH_ATTACHMENT);
    }
    
    void FrameBuffer::attachStencilBuffer(const TexturePtr& buffer)
    {
      attachStencilBuffer(Attachment::create(buffer));
    }
    
    void FrameBuffer::attachStencilBuffer(const AttachmentPtr& buffer)
    {
      stencilBuffer = buffer;
      buffer->attach(GL_STENCIL_ATTACHMENT);
    }
    
    GLenum FrameBuffer::status()
    {
      return glCheckFramebufferStatus(GL_FRAMEBUFFER); GLASSERT;
    }

    bool FrameBuffer::isComplete()
    {
      return (GL_FRAMEBUFFER_COMPLETE == status());
    }

    void FrameBuffer::check()
    {

#define CB(v) case v:EOUT("inconsistent framebuffer: "<<#v);break;

      GLenum err = status();
      switch (err)
      {
        case GL_FRAMEBUFFER_COMPLETE:
//          DOUT("Framebuffer ok");
          break;
        CB(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
        CB(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
        CB(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
        CB(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER);
        CB(GL_FRAMEBUFFER_UNSUPPORTED);
        default:
          EOUT("inconsistent framebuffer: "<<err);
      }
    }
  
    void FrameBuffer::bind()
    {
      glBindFramebuffer(GL_FRAMEBUFFER, buffer);GLASSERT;
    }
}
