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

#include "lost/VertexAttribute.h"
#include "lost/Uniform.h"

namespace lost
{

/** NOTE: in order to minimise side effects of functions, you manually have to call 
 *  buildUniformMap() and buildVertexAttributeMap() after you compiled, linked and enabled the shader.
 */
struct ShaderProgram
{
public:
typedef map<string, Uniform>          UniformMap;
typedef map<string, VertexAttribute>  VertexAttributeMap;
typedef list<ShaderPtr>                    ShaderList;

  ShaderProgram();
  virtual ~ShaderProgram();
  static ShaderProgramPtr create() { return ShaderProgramPtr(new ShaderProgram); }

  Uniform& uniform(const string& inName);
  Uniform& operator[](const string& inName);
  void attach(ShaderPtr& inShader);
  void detachAllShaders(); // detaches all shaders, throwing awa all internal references to shader objects
  void link();
  bool linked();
  void enable();
  void disable();
  // returns information about the current program state
  GLint param(GLenum paramName);
  string log();
  void validate();
  bool validated();
  GLenum numericalType(const string& inName);

  void setInt(const string& inName, GLint inVal);
  void setFloat(const string& inName, float inVal);
  void setBool(const string& inName, bool inVal);
  void set(const string& inName, const Color& inVal);
  void set(const string& inName, const Vec4& inVal);
  void set(const string& inName, const Vec2& inVal);
  void set(const string& inName, const Vec3& inVal);
  void set(const string& inName, const Matrix& inVal);
  bool hasUniform(const string& name);
  bool hasAttribute(const string& name);

  UniformMap& uniformMap();

  // uniform and vertexAttribute maps need to be rebuild each time you link the shader program
  void buildUniformMap();
  void buildVertexAttributeMap();

  GLuint              program;
  UniformMap          name2uniform;
  VertexAttributeMap  name2vertexAttribute;
  ShaderList          shaders;
};

}

