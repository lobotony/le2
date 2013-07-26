#ifndef LOST_INPUTEVENTSYSTEM_H
#define LOST_INPUTEVENTSYSTEM_H

struct input_event;

namespace lost
{

struct InputEventSystem
{
  InputEventSystem();

  const char* state2string();
  void parse(struct input_event* events, u32 num);
  void emitEvent(struct input_event* ev);
  void run(const char* deviceName);
  void getTouchBounds(int fd);
  void resetLastUpdateTime();

  const static u32 ST_NONE=0;
  const static u32 ST_TOUCH_UPDATE=1;
  const static u32 ST_TOUCH_DOWN=2;
  const static u32 ST_TOUCH_UP=3;


  // all coordinates and dimensions in raw device units
  // conversion will be performed upon 
  f32 minX;
  f32 maxX;
  f32 dx;
  f32 minY;
  f32 maxY;
  f32 dy;
  f32 currentX;
  f32 currentY;

  f32 lastX;
  f32 lastY;
  struct timeval lastUpdateTime;
  u32 previousState;


  f32 displayWidth;
  f32 displayHeight;

  u32 currentState;

  void run();
};

}

#endif


