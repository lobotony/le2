#pragma once

#include "lost/PlatformType.h"
#include "lost/Log.h"

// in case of iphone, this header includes the OpenGL ES interfaces
// every other platform gets the standard gl headers plus helpers provided
// by GLee (extension handling) and GLFW (platform abstraction)
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || defined ANDROID || defined LOST_PLATFORM_RPI
  #define OPENGL_ES_1_1 11
  #define OPENGL_ES_2_0 20

  #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
    #define VERSION_OPENGL_ES OPENGL_ES_1_1
  #elif defined LOST_PLATFORM_RPI
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #define VERSION_OPENGL_ES OPENGL_ES_2_0
    #define GL_DEPTH_COMPONENT24 GL_DEPTH_COMPONENT24_OES
    #define GL_DEPTH_COMPONENT32 GL_DEPTH_COMPONENT32_OES    
   #else
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #define VERSION_OPENGL_ES OPENGL_ES_2_0
  #endif

  #define TARGET_OPENGL    0
  #define TARGET_OPENGL_ES 1
#else
  #if defined WIN32
    #include "GLee.h"
    #include <GL/gl.h>
    #include <GL/glext.h>
    #include <GL/glu.h>
    #include <wingdi.h>
  #elif defined LOST_PLATFORM_RPI
    #include <GLES2/gl2.h>
    #include <EGL/egl.h>
  #elif defined linux
    #include <GL/gl.h>
    #include <GL/glx.h>
    #include <GL/glext.h>
    #include <GL/glu.h>
  #elif defined __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
    #include <OpenGL/CGLTypes.h>
    #include <OpenGL/CGLCurrent.h>
    #include <OpenGL/glu.h>
  #endif

  #define TARGET_OPENGL    1
  #define TARGET_OPENGL_ES 0
#endif

namespace lost
{
  lost::string getGlErrorAsString(GLenum err);
  lost::string enum2string(GLenum inVal);
}
  
#if defined(LOST_GL_DEBUG)
#   define GLDEBUG { GLenum err = glGetError(); if(err != GL_NO_ERROR) {DOUT("GL ERROR:"+getGlErrorAsString(err));};}
#   define GLASSERT {\
GLenum err = glGetError(); \
if(err != GL_NO_ERROR) { \
ASSERT(false, "GL error: " << getGlErrorAsString(err)); \
}}
#else
#   define GLDEBUG
#   define GLASSERT
#endif


