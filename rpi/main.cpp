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
#include "esUtil.h"
#include "lost/Engine.h"

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   lost::Engine::instance()->doStartup();
   return GL_TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   lost::Engine::instance()->doUpdate();
}

int main ( int argc, char *argv[] )
{
  DOUT("YEAH!");
   ESContext esContext;

   esInitContext ( &esContext );

   esCreateWindow ( &esContext, "Hello Triangle", 1280, 800, ES_WINDOW_RGB | ES_WINDOW_ALPHA );

   if ( !Init ( &esContext ) )
      return 0;

   esRegisterDrawFunc ( &esContext, Draw );

   esMainLoop ( &esContext );
}
