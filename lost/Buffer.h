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

#pragma once

namespace lost
{
struct Buffer
{
  GLuint buffer;
  GLenum target;
  GLint size;
  GLenum type;
  unsigned long numElements; // number of verts/colors/texcoords in array

  Buffer();
  virtual ~Buffer();
  virtual void bind();
  virtual void bufferData(GLsizeiptr size, const GLvoid* data, GLenum usage=GL_STATIC_DRAW) ;
  virtual void bindBufferData(GLsizeiptr size, const GLvoid* data, GLenum usage=GL_STATIC_DRAW);
  virtual void bufferSubData(GLintptr offset,GLsizeiptr sizeInBytes,	const GLvoid* data) ;
  virtual void bindBufferSubData(GLintptr offset,GLsizeiptr size,	const GLvoid* data);

  virtual void drawArrays(GLenum mode, GLint first=0, GLsizei count=-1);

  // indices: used as byte offset into buffer when element array is bound
  virtual void drawElements(GLenum mode, GLsizei count=-1, const GLvoid* indices = 0);
  virtual void drawElements(GLenum  	     mode,
                            GLsizei  	     count,
                            GLenum  	     inType,
                            const GLvoid * indices);

  virtual void bind(GLenum target);

  virtual void bufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);

  virtual void bufferSubData(GLenum  	target,
                             GLintptr  	offset,
                             GLsizeiptr  	size,
                             const GLvoid *  	data);

  virtual void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride=0, const GLvoid* pointer=0);

};

}

