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

/**
 * Note:
 *
 *   - application::gl::Context is designed to live within an application::Window
 *   - to reduce overhead the platform specific part expects that an OpenGL context already exists and is set as the thread's current context!
 *   - the ctor gets the current context and uses that handle for it's complete lifecycle (e.g. makeCurrent, swapBuffers)
 *
 * There is only one proper way to initialize an application::gl::Context:
 *   1.) create an application::Window (it'll create and activate an OpenGL context) or create an OpenGL context and make it the thread's current!
 *   2.) create the application::gl::Context
 *
 */

#ifndef LOST_CONTEXT_H
#define LOST_CONTEXT_H

namespace lost
{
struct Context
{
// private for now, deliberately no getters
private:  
  bool vertexArrayEnabled;
  bool normalArrayEnabled;
  bool colorArrayEnabled;
  bool texCoordArrayEnabled;
  bool depthTestEnabled;
  bool blendEnabled;
  GLenum currentBlendFuncSource;
  GLenum currentBlendFuncDestination;
  bool scissorEnabled;
  Rect currentScissorRect;
  bool texture2DEnabled;
  Color currentClearColor;
  CameraPtr currentCam;
  Rect currentViewport;
  GLenum currentActiveTexture;
  ShaderProgramPtr currentShader;
  bool cullEnabled;
  GLenum cullFaceMode;
  static const uint32_t _maxTextures = 32;
  GLuint activeTextures[_maxTextures]; // hardcoded 32 texture limit, index represents value set with glActiveTexture
  static const uint32_t _maxVertexAttributes = 32;
  bool _vertexAttributeEnabled[_maxVertexAttributes]; // true if glEnableVertexAttribArray was called for index i
  bool _vertexAttributeRequired[_maxVertexAttributes]; // true if the vertex attribute enable state changed

  /**
   * hidden ctor/dtor utility methods for platform specific stuff
   */
  void initialize();
  void finalize();

  GLuint m_defaultFrameBuffer;
  vector<Rect> _scissorRectStack;
  
  vector<Matrix> modelViewStack;
  
public:
  Context();
  ~Context();

  static Context* instance(); // returns the context instance created by the application. 

  void bindDefaultFramebuffer();
  void defaultFramebuffer(GLuint fbo);
  GLuint defaultFramebuffer();
  
  void pushModelViewMatrix(const Matrix& matrix);
  void popModelViewMatrix();

  void depthTest(bool enable);
  void blend(bool enable);
  void blendFunc(GLenum src, GLenum dest);

  void scissor(bool enable);
  void scissorRect(const Rect& rect); // sets the current scissoring region to rect
  void pushScissorRect(const Rect& v); // pushes a rect on the scissor stack, setting it for scissoring and enabling scissor if required
  void pushClippedScissorRect(const Rect& v); // same as pushScissorRect, but clips the new rect against a previews one if one exists.
  void popScissorRect(); // pops the current scissor rect fro the stack, disabling scissoring if it was the last one.
  
  void cull(bool enable);
  void cullFace(GLenum v);
  
  void clearColor(const Color& col); // sets the current clear color to col
  
  void camera(const CameraPtr& cam);
  void viewport(const Rect& val);
  void clear(GLbitfield flags);    
  
  void activeTexture(GLenum tex); // sets the currently active texture unit
  void bindTexture(GLuint tex, bool override); // binds tex to currently active texture unit. Override is required to rebind destroyed and immediately recreated texture objects 
  void bindTextures(const vector<TexturePtr>& textures); // binds textures to the units equivalent to the index in the vector
  void material(const MaterialPtr& mat); // applies the Material parameters to the state
  
  void draw(const MeshPtr& mesh);
  void shader(const ShaderProgramPtr& prog); // makes prog the active shader, switching the previous active shader off. null values are ok.
  // writes the current framebuffer with the current viewport configurtaion to a file as a tga, with optional alpha channel.
  void writeScreenshot(const string& fullPathName, bool withAlphaChannel);
    
  void bind(Buffer* buffer);
  void applyUniforms(UniformBlock* ub); // applies a uniform block to the current shader
  void vertexAttributeEnable(uint32_t idx, bool enable);
  void clearVertexAttributeEnabled();
  void clearVertexAttributeRequired();
  void disableUnrequiredVertexAttributes();
};
}

#endif
