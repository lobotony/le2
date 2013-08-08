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

#include "lost/RenderBuffer.h"

namespace lost
{

RenderBuffer::RenderBuffer()
{
  glGenRenderbuffers(1, &buffer);GLASSERT;
}

GLint RenderBuffer::width() { return param(GL_RENDERBUFFER_WIDTH); }
GLint RenderBuffer::height() { return param(GL_RENDERBUFFER_HEIGHT); }
GLint RenderBuffer::internalFormat() { return param(GL_RENDERBUFFER_INTERNAL_FORMAT); }
GLint RenderBuffer::redSize() { return param(GL_RENDERBUFFER_RED_SIZE); }
GLint RenderBuffer::greenSize() { return param(GL_RENDERBUFFER_GREEN_SIZE); }
GLint RenderBuffer::blueSize() { return param(GL_RENDERBUFFER_BLUE_SIZE); }
GLint RenderBuffer::alphaSize() { return param(GL_RENDERBUFFER_ALPHA_SIZE); }
GLint RenderBuffer::depthSize() { return param(GL_RENDERBUFFER_DEPTH_SIZE); }
GLint RenderBuffer::stencilSize() { return param(GL_RENDERBUFFER_STENCIL_SIZE); }

void RenderBuffer::enable()
{
  // FIXME: should bind via Context
  glBindRenderbuffer(GL_RENDERBUFFER, buffer);GLASSERT;
}

void RenderBuffer::disable()
{
  // FIXME: should bind via Context
  glBindRenderbuffer(GL_RENDERBUFFER, 0);GLASSERT;
}

void RenderBuffer::storage(GLenum inInternalFormat, GLsizei inWidth, GLsizei inHeight)
{
  glRenderbufferStorage(GL_RENDERBUFFER, inInternalFormat, inWidth, inHeight);GLASSERT;
}

GLint RenderBuffer::param(GLenum paramName)
{
  GLint result;
  glGetRenderbufferParameteriv(buffer, paramName, &result);GLASSERT;
  return result;
}

RenderBuffer::~RenderBuffer()
{
  // FIXME: should notify Context of imminent destruction
  glDeleteRenderbuffers(1, &buffer);GLASSERT;
}

}
