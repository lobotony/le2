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

#include "lost/Buffer.h"
#include "lost/Context.h"

namespace lost
{

  Buffer::Buffer()
  {
    glGenBuffers(1, &buffer);GLASSERT;
    numElements = 0;
  }

   Buffer::~Buffer()
  {
    glDeleteBuffers(1, &buffer);GLDEBUG;// must not throw
  }

   void Buffer::bind() { Buffer::bind(target); }
   void Buffer::bufferData(GLsizeiptr size, const GLvoid* data, GLenum usage) {Buffer::bufferData(target, size, data, usage);}
   void Buffer::bindBufferData(GLsizeiptr size, const GLvoid* data, GLenum usage) {bind();Buffer::bufferData(target, size, data, usage);}
   void Buffer::bufferSubData(GLintptr offset,GLsizeiptr sizeInBytes,	const GLvoid* data) {Buffer::bufferSubData(target, offset, size, data);};
   void Buffer::bindBufferSubData(GLintptr offset,GLsizeiptr size,	const GLvoid* data) {bind(); Buffer::bufferSubData(target, offset, size, data);};

   void Buffer::drawArrays(GLenum mode, GLint first, GLsizei count)
  {
    if(count == -1)
      count = (GLsizei)numElements;
    glDrawArrays(mode, first, count);GLASSERT;
  }

  // indices: used as byte offset into buffer when element array is bound
   void Buffer::drawElements(GLenum mode, GLsizei count, const GLvoid* indices)
  {
    if(count == -1)
      count = (GLsizei)numElements;
    drawElements(mode, count, this->type, indices);
  }

   void Buffer::drawElements(GLenum  	     mode,
                            GLsizei  	     count,
                            GLenum  	     inType,
                            const GLvoid * indices)
  {
    glDrawElements(mode, count, inType, indices);GLASSERT;
  }

  void Buffer::bind(GLenum inTarget)
  {
    //glBindBuffer(inTarget, buffer);GLASSERT;
    Context::instance()->bind(this);
  }

   void Buffer::bufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
  {
    glBufferData(target, size, data, usage);GLASSERT;
  }

   void Buffer::bufferSubData(GLenum  	target,
                             GLintptr  	offset,
                             GLsizeiptr  	size,
                             const GLvoid *  	data)
  {
    glBufferSubData(target, offset, size, data);GLASSERT;
  }

  void Buffer::vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
  {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);GLASSERT;
  }
}
