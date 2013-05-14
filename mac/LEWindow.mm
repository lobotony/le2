#import "LEWindow.h"
#include "lost/Engine.h"
#include "lost/EventPool.h"
#include "lost/EventQueue.h"

extern lost::Engine* _engineInstance;

@implementation LEWindow

-(void)keyDown:(NSEvent *)theEvent
{
  lost::Event* event = _engineInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_KeyDownEvent;
  _engineInstance->eventQueue->addEvent(event);
}

-(void)keyUp:(NSEvent *)theEvent
{
  lost::Event* event = _engineInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_KeyUpEvent;
  _engineInstance->eventQueue->addEvent(event);
}

- (void)mouseDown:(NSEvent *)theEvent
{
  lost::Event* event = _engineInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_MouseDownEvent;
  _engineInstance->eventQueue->addEvent(event);
}

- (void)mouseUp:(NSEvent *)theEvent
{
  lost::Event* event = _engineInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_MouseUpEvent;
  _engineInstance->eventQueue->addEvent(event);
}

- (void)mouseMoved:(NSEvent *)theEvent
{
  lost::Event* event = _engineInstance->eventPool->borrowEvent();
  event->base.type = lost::ET_MouseMoveEvent;
  _engineInstance->eventQueue->addEvent(event);
}

-(BOOL)canBecomeKeyWindow
{
  return YES;
}


@end

