#import "LEAppDelegate.h"
#import "LEGLView.h"
#include "lost/Engine.h"

extern lost::Engine* _engineInstance;

@implementation LEAppDelegate

@synthesize window;

- (void) quitAction: (id)sender
{
  LEGLView* view = (LEGLView*)[window contentView];
  CVDisplayLinkStop([view displayLink]);
  _engineInstance->doShutdown();
  [[NSApplication sharedApplication] terminate: nil];
}

@end
