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

#include "lost/ShaderProgram.h"
#include "lost/Shader.h"
#include "lost/Uniform.h"
#include "lost/Context.h"

namespace lost
{

ShaderProgram::ShaderProgram()
{
  program = glCreateProgram();GLASSERT;
}

ShaderProgram::~ShaderProgram()
{
  Context::instance()->shaderprogramDying(this);
  glDeleteProgram(program);
}

Uniform& ShaderProgram::uniform(const string& inName)
{
  static Uniform dummy; // to suppress compiler warning
  ShaderProgram::UniformMap::iterator pos = name2uniform.find(inName);
  if(pos != name2uniform.end())
  {
    return pos->second;
  }
  else
  {
    ASSERT(false, "couldn't find uniform with name: '"+inName+"'");
    return dummy; // to suppress compiler warning
  }
}

Uniform& ShaderProgram::operator[](const string& inName)
{
  return uniform(inName);
}

void ShaderProgram::attach(ShaderPtr& inShader)
{
  glAttachShader(program, inShader->shader);GLASSERT;
  shaders.push_back(inShader);
}

void ShaderProgram::detachAllShaders()
{
  for(ShaderList::iterator i=shaders.begin(); i!=shaders.end(); ++i)
  {
    glDetachShader(program, (*i)->shader);GLASSERT;
  }
  shaders.clear();
}

void ShaderProgram::link()
{
  glLinkProgram(program);GLASSERT;
}

bool ShaderProgram::linked()
{
  return param(GL_LINK_STATUS) == 0 ? false : true;
}

void ShaderProgram::enable()
{
  Context::instance()->enableShader(this);
}

void ShaderProgram::disable()
{
  Context::instance()->disableShader();
}

// returns information about the current program state
GLint ShaderProgram::param(GLenum paramName)
{
  GLint result;
  glGetProgramiv(program, paramName, &result);GLASSERT;
  return result;
}

string ShaderProgram::log()
{
  GLint size = param(GL_INFO_LOG_LENGTH);
  string result;
  if(size > 0)
  {
    shared_array<char> buffer(new char[size]);
    GLsizei length = 0;
    glGetProgramInfoLog(program, size, &length, buffer.get());GLASSERT;
    result = string(buffer.get(), buffer.get()+length);
  }
  return result;
}

void ShaderProgram::validate()
{
  glValidateProgram(program);GLASSERT;
}

bool ShaderProgram::validated()
{
  return (param(GL_VALIDATE_STATUS) == GL_TRUE) ? true : false;
}

GLenum ShaderProgram::numericalType(const string& inName) { return uniform(inName).glType; } 

void ShaderProgram::setInt(const string& inName, GLint inVal) {uniform(inName).setInt(inVal);}
void ShaderProgram::setFloat(const string& inName, float inVal) {uniform(inName).setFloat(inVal);}
void ShaderProgram::setBool(const string& inName, bool inVal) {uniform(inName).setBool(inVal);}
void ShaderProgram::set(const string& inName, const Color& inVal) {uniform(inName).set(inVal);}
void ShaderProgram::set(const string& inName, const Vec4& inVal) {uniform(inName).set(inVal);}
void ShaderProgram::set(const string& inName, const Vec2& inVal) {uniform(inName).set(inVal);}
void ShaderProgram::set(const string& inName, const Vec3& inVal) {uniform(inName).set(inVal);}
void ShaderProgram::set(const string& inName, const Matrix& inVal) {uniform(inName).set(inVal);}

bool ShaderProgram::hasUniform(const string& name)
{
  return (name2uniform.find(name) != name2uniform.end());
}

bool ShaderProgram::hasAttribute(const string& name)
{
  return (name2vertexAttribute.find(name) != name2vertexAttribute.end());
}

ShaderProgram::UniformMap& ShaderProgram::uniformMap()
{
  return name2uniform;
}

void ShaderProgram::buildUniformMap()
{
  name2uniform.clear();
  GLint numUniforms = param(GL_ACTIVE_UNIFORMS);
  int bufferSize = param(GL_ACTIVE_UNIFORM_MAX_LENGTH);
  shared_array<char> buffer(new char[bufferSize]);
  GLint writtenBytes = 0;
  GLenum type;
  GLint size;

//  DOUT("--- Uniforms");
  for(GLint i=0; i<numUniforms; ++i)
  {
    glGetActiveUniform(program, i, bufferSize, &writtenBytes, &size, &type, buffer.get());
    GLint loc = glGetUniformLocation(program, buffer.get());
    Uniform uniform(buffer.get(), i, type, size, loc);
    name2uniform[uniform.name] = uniform;
//    DOUT(i << " : " << string(buffer.get(), buffer.get()+writtenBytes) << " size:"<<size << " type:"<<lost::gl::utils::enum2string(type)<<" location:"<<loc);
  }
}

void ShaderProgram::buildVertexAttributeMap()
{
  name2vertexAttribute.clear();
  GLint numAttributes = param(GL_ACTIVE_ATTRIBUTES);
  int bufferSize = param(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
  shared_array<char> buffer(new char[bufferSize]);
  GLint writtenBytes = 0;
  GLenum type;
  GLint size;

//  DOUT("--- Attributes");
  for(GLint i=0; i<numAttributes; ++i)
  {
    glGetActiveAttrib(program, i, bufferSize, &writtenBytes, &size, &type, buffer.get());
    GLint loc = glGetAttribLocation(program, buffer.get());
    VertexAttribute attribute(buffer.get(), i, type, size, loc);
    name2vertexAttribute[attribute.name] = attribute;
//    DOUT(i << " : " << string(buffer.get(), buffer.get()+writtenBytes) << " size:"<<size << " type:"<<lost::gl::utils::enum2string(type)<<" location:"<<loc);
  }
}

}
