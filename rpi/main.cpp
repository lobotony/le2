//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

#include <stdlib.h>
#include "lost/Engine.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include  "bcm_host.h"
#include "vc_dispmanx_types.h"
#include <sstream>

using namespace std;

EGL_DISPMANX_WINDOW_T nativewindow;

EGLDisplay display;
EGLContext context;
EGLSurface surface;

string eglErrorString()
{
  EGLint v = eglGetError();
  ostringstream os;
  os << "0x" <<  std::hex  << v;
  return os.str();
}

#define EGLASSERT(c) ASSERT(c, " error:"<<eglErrorString())

void createNativeWindow() 
{
  DISPMANX_ELEMENT_HANDLE_T dispman_element;
  DISPMANX_DISPLAY_HANDLE_T dispman_display;
  DISPMANX_UPDATE_HANDLE_T dispman_update;
  VC_RECT_T dst_rect;
  VC_RECT_T src_rect;

  uint32_t display_width;
  uint32_t display_height;

  int32_t success = 0;
  success = graphics_get_display_size(0 /* LCD */, 
                                      &display_width, 
                                      &display_height);
  ASSERT(success >= 0, "couldn't get display size");
  DOUT("display size: "<<display_width<< " x "<<display_height);

  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.width = display_width;
  dst_rect.height = display_height;
    
  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = display_width << 16;
  src_rect.height = display_height << 16;   

  dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
  dispman_update = vc_dispmanx_update_start( 0 );
         
  // set alpha to prevent surfaces beneath GL context to show through when GL context uses alpha channel
  VC_DISPMANX_ALPHA_T alpha = { DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS,255,0 };
  dispman_element = vc_dispmanx_element_add(dispman_update,
                                            dispman_display,
                                            0/*layer*/, 
                                            &dst_rect, 
                                            0/*src*/,
                                            &src_rect, 
                                            DISPMANX_PROTECTION_NONE, 
                                            &alpha, 
                                            0/*clamp*/, 
                                            (DISPMANX_TRANSFORM_T)0/*transform*/);
  nativewindow.element = dispman_element;
  nativewindow.width = display_width;
  nativewindow.height = display_height;
  vc_dispmanx_update_submit_sync( dispman_update );   
}

int main ( int argc, char *argv[] )
{
  DOUT("===========================================================");
  DOUT("===========================================================");
  DOUT("===========================================================");
  DOUT("===========================================================");

  bcm_host_init();

  EGLint attribList[] =
  {
     EGL_RED_SIZE,       8,
     EGL_GREEN_SIZE,     8,
     EGL_BLUE_SIZE,      8,
     EGL_ALPHA_SIZE,     8,
//     EGL_DEPTH_SIZE,     24,
     EGL_NONE
  };

  createNativeWindow();

  EGLint numConfigs;
  EGLint majorVersion;
  EGLint minorVersion;
  EGLConfig config;
  EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
   
  EGLASSERT((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) != EGL_NO_DISPLAY);
  EGLASSERT(eglInitialize(display, &majorVersion, &minorVersion));
  EGLASSERT(eglGetConfigs(display, NULL, 0, &numConfigs));
  EGLASSERT(eglChooseConfig(display, attribList, &config, 1, &numConfigs));
  EGLASSERT((surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)&nativewindow, NULL)) != EGL_NO_SURFACE);
  EGLASSERT((context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs)) != EGL_NO_CONTEXT);
  EGLASSERT(eglMakeCurrent(display, surface, surface, context));
   
  lost::Engine::instance()->doStartup();
  while(true)
  {
    lost::Engine::instance()->doUpdate();
    eglSwapBuffers(display, surface);
  }
}
