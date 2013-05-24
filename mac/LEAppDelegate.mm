#import "LEAppDelegate.h"
#import "LEGLView.h"
#include "lost/Engine.h"
#include "lost/EventPool.h"
#include "lost/EventQueue.h"
#include "lost/Event.h"

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

- (void)windowDidResize:(NSNotification *)notification
{
  [self sendWindowResizeEvent];
}

-(void)sendWindowResizeEvent
{
  NSRect curFrame = [self.window contentRectForFrameRect:self.window.frame];
//  DOUT("w:"<<int(curFrame.size.width)<<" h:"<<int(curFrame.size.height));
  lost::Event* event = _engineInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_WindowResize;
  event->windowResizeEvent.width = curFrame.size.width;
  event->windowResizeEvent.height = curFrame.size.height;
  _engineInstance->eventQueue->addEventToNextQueue(event);
}

@end

