#import "LEWindow.h"
#include "lost/Application.h"
#include "lost/EventPool.h"
#include "lost/EventQueue.h"

extern lost::Application* _appInstance;

@implementation LEWindow

-(void)keyDown:(NSEvent *)theEvent
{
  lost::Event* event = _appInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_KeyDownEvent;
  _appInstance->eventQueue->addEventToNextQueue(event);
}

-(void)keyUp:(NSEvent *)theEvent
{
  lost::Event* event = _appInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_KeyUpEvent;
  _appInstance->eventQueue->addEventToNextQueue(event);
}

- (void)mouseDown:(NSEvent *)theEvent
{
  lost::Event* event = _appInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_MouseDownEvent;
  _appInstance->eventQueue->addEventToNextQueue(event);
}

- (void)mouseUp:(NSEvent *)theEvent
{
  lost::Event* event = _appInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_MouseUpEvent;
  _appInstance->eventQueue->addEventToNextQueue(event);
}

- (void)mouseMoved:(NSEvent *)theEvent
{
  NSPoint eventLocation = [theEvent locationInWindow];
  NSPoint center = [self.contentView convertPoint:eventLocation fromView:nil];
//  DOUT("move pos "<<center.x<<" "<<center.y);

  lost::Event* event = _appInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_MouseMoveEvent;
  event->mouseEvent.type = lost::ET_MouseMoveEvent;
  event->mouseEvent.x = center.x;
  event->mouseEvent.y = center.y;
  _appInstance->eventQueue->addEventToNextQueue(event);
}

-(BOOL)canBecomeKeyWindow
{
  return YES;
}


@end

