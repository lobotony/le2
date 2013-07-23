#import "LEAppDelegate.h"
#import "LEGLView.h"
#include "lost/Application.h"
#include "lost/EventPool.h"
#include "lost/EventQueue.h"
#include "lost/Event.h"

extern lost::Application* _appInstance;

@implementation LEAppDelegate

@synthesize window;

- (void) quitAction: (id)sender
{
  LEGLView* view = (LEGLView*)[window contentView];
  CVDisplayLinkStop([view displayLink]);
  _appInstance->doShutdown();
  [[NSApplication sharedApplication] terminate: nil];
}

- (void)windowDidResize:(NSNotification *)notification
{
  [self sendWindowResizeEvent];
}

-(void)sendWindowResizeEvent
{
  NSRect curFrame = [self.window contentRectForFrameRect:self.window.frame];
  DOUT("w:"<<int(curFrame.size.width)<<" h:"<<int(curFrame.size.height));
  lost::Event* event = _appInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_WindowResize;
  _appInstance->windowSize.width = curFrame.size.width;
  _appInstance->windowSize.height = curFrame.size.height;  
  _appInstance->eventQueue->addEventToNextQueue(event);
}

@end

