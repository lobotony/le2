#ifndef ESUTIL_H
#define ESUTIL_H

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#ifdef __cplusplus

extern "C" {
#endif   
#define ESUTIL_API
#define ESCALLBACK

#define ES_WINDOW_RGB           0
#define ES_WINDOW_ALPHA         1 
#define ES_WINDOW_DEPTH         2 
#define ES_WINDOW_STENCIL       4
#define ES_WINDOW_MULTISAMPLE   8

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef struct
{
    GLfloat   m[4][4];
} ESMatrix;

typedef struct _escontext
{

  void*       userData; /// Put your user data here...
  GLint       width;/// Window width
  GLint       height;/// Window height
  EGLNativeWindowType  hWnd; /// Window handle
  EGLDisplay  eglDisplay;/// EGL display
  EGLContext  eglContext;/// EGL context
  EGLSurface  eglSurface;/// EGL surface
  void (ESCALLBACK *drawFunc) ( struct _escontext * );
  void (ESCALLBACK *keyFunc) ( struct _escontext *, unsigned char, int, int );
  void (ESCALLBACK *updateFunc) ( struct _escontext *, float deltaTime );
} ESContext;


void ESUTIL_API esInitContext ( ESContext *esContext );
GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags );
void ESUTIL_API esMainLoop ( ESContext *esContext );
void ESUTIL_API esRegisterDrawFunc ( ESContext *esContext, void (ESCALLBACK *drawFunc) ( ESContext* ) );
void ESUTIL_API esRegisterUpdateFunc ( ESContext *esContext, void (ESCALLBACK *updateFunc) ( ESContext*, float ) );
void ESUTIL_API esRegisterKeyFunc ( ESContext *esContext, void (ESCALLBACK *drawFunc) ( ESContext*, unsigned char, int, int ) );

#ifdef __cplusplus
}
#endif

#endif // ESUTIL_H
