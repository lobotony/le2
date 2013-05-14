#import "LEGLView.h"
#import "lost/Engine.h"

extern lost::Engine* _engineInstance;

static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
  LEGLView* glview = (LEGLView*)displayLinkContext;
  NSOpenGLContext* glcontext = [glview openGLContext];
  [glcontext makeCurrentContext];
  _engineInstance->doUpdate();
  [glcontext flushBuffer];
  
  return kCVReturnSuccess;
}


@implementation LEGLView

-(id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format
{
  self = [super initWithFrame:frameRect pixelFormat:format];
  if(self)
  {
    
  }
  return self;
}

-(void)dealloc
{
  CVDisplayLinkRelease(_displayLink);
  [super dealloc];
}

- (void)prepareOpenGL
{
  // Synchronize buffer swaps with vertical refresh rate
  GLint swapInt = 1;
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval]; 
  
  // Create a display link capable of being used with all active displays
  CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
  
  // Set the renderer output callback function
  CVDisplayLinkSetOutputCallback(_displayLink, &MyDisplayLinkCallback, self);
  
  // Set the display link for the current renderer
  CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
  CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
  CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(_displayLink, cglContext, cglPixelFormat);
  
  // Activate the display link
  CVDisplayLinkStart(_displayLink);
}

-(CVDisplayLinkRef)displayLink
{
  return _displayLink;
}


@end

