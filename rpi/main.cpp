//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this 
//    example is to demonstrate the basic concepts of 
//    OpenGL ES 2.0 rendering.
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

typedef struct _escontext
{
  GLint       width;/// Window width
  GLint       height;/// Window height
  EGLNativeWindowType  hWnd; /// Window handle
  EGLDisplay  eglDisplay;/// EGL display
  EGLContext  eglContext;/// EGL context
  EGLSurface  eglSurface;/// EGL surface

} ESContext;

string eglErrorString()
{
  EGLint v = eglGetError();
  ostringstream os;
  os << "0x" <<  std::hex  << v;
  return os.str();
}

#define EGLASSERT(c) ASSERT(c, " error:"<<eglErrorString())

void CreateEGLContext ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                              EGLContext* eglContext, EGLSurface* eglSurface,
                              EGLint attribList[])
{
  EGLint numConfigs;
  EGLint majorVersion;
  EGLint minorVersion;
  EGLDisplay display;
  EGLContext context;
  EGLSurface surface;
  EGLConfig config;
  EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
   
  EGLASSERT((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) != EGL_NO_DISPLAY);
  EGLASSERT(eglInitialize(display, &majorVersion, &minorVersion));
  EGLASSERT(eglGetConfigs(display, NULL, 0, &numConfigs));
  EGLASSERT(eglChooseConfig(display, attribList, &config, 1, &numConfigs));
  EGLASSERT((surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL)) != EGL_NO_SURFACE);
  EGLASSERT((context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs)) != EGL_NO_CONTEXT);
  EGLASSERT(eglMakeCurrent(display, surface, surface, context));
   
  *eglDisplay = display;
  *eglSurface = surface;
  *eglContext = context;
} 

///
//  WinCreate() - RaspberryPi, direct surface (No X, Xlib)
//
//      This function initialized the display and window for EGL
//
void WinCreate(ESContext *esContext) 
{
   int32_t success = 0;

   DISPMANX_ELEMENT_HANDLE_T dispman_element;
   DISPMANX_DISPLAY_HANDLE_T dispman_display;
   DISPMANX_UPDATE_HANDLE_T dispman_update;
   VC_RECT_T dst_rect;
   VC_RECT_T src_rect;
   
   uint32_t display_width;
   uint32_t display_height;

   // create an EGL window surface, passing context width/height
   success = graphics_get_display_size(0 /* LCD */, &display_width, &display_height);
   if ( success < 0 )
   {
      // FIXME: assert
   }

   VC_DISPMANX_ALPHA_T alpha = { DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS,255,0 };
   
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
         
   dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
      0/*layer*/, &dst_rect, 0/*src*/,
      &src_rect, DISPMANX_PROTECTION_NONE, &alpha, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);
      
   nativewindow.element = dispman_element;
   nativewindow.width = display_width;
   nativewindow.height = display_height;
   vc_dispmanx_update_submit_sync( dispman_update );
   
   esContext->hWnd = &nativewindow;
}

int main ( int argc, char *argv[] )
{
  DOUT("===========================================================");
  DOUT("===========================================================");
  DOUT("===========================================================");
  DOUT("===========================================================");

  bcm_host_init();

  ESContext esContext;
  memset( &esContext, 0, sizeof( ESContext) );

  EGLint attribList[] =
  {
     EGL_RED_SIZE,       8,
     EGL_GREEN_SIZE,     8,
     EGL_BLUE_SIZE,      8,
     EGL_ALPHA_SIZE,     8,
//     EGL_DEPTH_SIZE,     24,
     EGL_NONE
  };

  WinCreate(&esContext);

  CreateEGLContext(esContext.hWnd,
                  &esContext.eglDisplay,
                  &esContext.eglContext,
                  &esContext.eglSurface,
                  attribList);

  lost::Engine::instance()->doStartup();
  while(true)
  {
    lost::Engine::instance()->doUpdate();
    eglSwapBuffers(esContext.eglDisplay, esContext.eglSurface);
  }
}
