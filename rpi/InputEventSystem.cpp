#include "InputEventSystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>

#include "lost/Application.h"
#include "lost/Event.h"
#include "lost/EventPool.h"
#include "lost/EventQueue.h"

extern lost::Application* _appInstance;

namespace lost
{

const char* type2string(__u16 t)
{
  const char* result = "?";

  switch(t)
  {
    case EV_SYN:result="EV_SYN";break;
    case EV_KEY:result="EV_KEY";break;
    case EV_REL:result="EV_REL";break;
    case EV_ABS:result="EV_ABS";break;
    case EV_MSC:result="EV_MSC";break;
    case EV_LED:result="EV_LED";break;
    case EV_SND:result="EV_SND";break;
    case EV_REP:result="EV_REP";break;
    case EV_FF :result="EV_FF";break;
    case EV_PWR:result="EV_PWR";break;
    case EV_FF_STATUS:result="EV_FF_STATUS";break;
    case EV_MAX:result="EV_MAX";break;
  }

  return result;
}

const char* abscode2string(__u16 code)
{
  const char* result = "?";

  #define CV(v) case v:result=#v;break;
  switch(code)
  {
    CV(ABS_X              );
    CV(ABS_Y              );
    CV(ABS_Z              );
    CV(ABS_RX             );
    CV(ABS_RY             );
    CV(ABS_RZ             );
    CV(ABS_THROTTLE       );
    CV(ABS_RUDDER         );
    CV(ABS_WHEEL          );
    CV(ABS_GAS            );
    CV(ABS_BRAKE          );
    CV(ABS_HAT0X          );
    CV(ABS_HAT0Y          );
    CV(ABS_HAT1X          );
    CV(ABS_HAT1Y          );
    CV(ABS_HAT2X          );
    CV(ABS_HAT2Y          );
    CV(ABS_HAT3X          );
    CV(ABS_HAT3Y          );
    CV(ABS_PRESSURE       );
    CV(ABS_DISTANCE       );
    CV(ABS_TILT_X         );
    CV(ABS_TILT_Y         );
    CV(ABS_TOOL_WIDTH     );
    CV(ABS_VOLUME     );
    CV(ABS_MISC       );
    CV(ABS_MT_SLOT    );
    CV(ABS_MT_TOUCH_MAJOR );
    CV(ABS_MT_TOUCH_MINOR );
    CV(ABS_MT_WIDTH_MAJOR );
    CV(ABS_MT_WIDTH_MINOR );
    CV(ABS_MT_ORIENTATION );
    CV(ABS_MT_POSITION_X  );
    CV(ABS_MT_POSITION_Y  );
    CV(ABS_MT_TOOL_TYPE   );
    CV(ABS_MT_BLOB_ID     );
    CV(ABS_MT_TRACKING_ID );
    CV(ABS_MT_PRESSURE    );
    CV(ABS_MT_DISTANCE    );
  }
  return result;
}

const char* syncode2string(__u16 code)
{
  const char* result = "?";

  #define CV(v) case v:result=#v;break;
  switch(code)
  {
    CV(SYN_REPORT   );
    CV(SYN_CONFIG   );
    CV(SYN_MT_REPORT);
    CV(SYN_DROPPED  );
  }
  return result;
}

const char* keycode2string(__u16 code)
{
  const char* result = "?";

  #define CV(v) case v:result=#v;break;
  switch(code)
  {
//    CV(BTN_DIGI);
    CV(BTN_TOOL_PEN);
    CV(BTN_TOOL_RUBBER);
    CV(BTN_TOOL_BRUSH);
    CV(BTN_TOOL_PENCIL);
    CV(BTN_TOOL_AIRBRUSH);
    CV(BTN_TOOL_FINGER);
    CV(BTN_TOOL_MOUSE);
    CV(BTN_TOOL_LENS);
    CV(BTN_TOOL_QUINTTAP);
    CV(BTN_TOUCH);
    CV(BTN_STYLUS);
    CV(BTN_STYLUS2);
    CV(BTN_TOOL_DOUBLETAP);
    CV(BTN_TOOL_TRIPLETAP);
    CV(BTN_TOOL_QUADTAP);
  }
  return result;
}

const char* code2string(__u16 type, __u16 code, const char* def)
{
  const char* result = "?";

  switch(type)
  {
    case EV_ABS:result = abscode2string(code); break;
    case EV_SYN:result = syncode2string(code); break;
    case EV_KEY:result = keycode2string(code); break;
    default:
      result = def;
  }
  
  return result;
}

void logEvent(struct input_event* ev)
{
  char defaultCodeString[64];
  sprintf(defaultCodeString, "%X", ev->code);

  const char* scode = code2string(ev->type, ev->code, defaultCodeString);
  DOUT((s64)ev->time.tv_sec << " " << (s64)ev->time.tv_usec << " " << type2string(ev->type) << " code:" << scode << " val:" << ev->value);

/*  if(ev->type != EV_ABS)
  {
    printf("%d %d %s code:%s  val:%d  \n", ev->time.tv_sec, ev->time.tv_usec, type2string(ev->type), scode, ev->value);
  }
  else if(ev->type)
  {
    const char* abscode = abscode2string(ev->code);
    printf("%d %d %s code:%s  val:%d  \n", ev->time.tv_sec, ev->time.tv_usec, type2string(ev->type), abscode, ev->value);
  }*/
}

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)    ((array[LONG(bit)] >> OFF(bit)) & 1)

bool hasAbsolutEvents(int fd)
{
  unsigned long bits[NBITS(KEY_MAX)];
  ioctl (fd, EVIOCGBIT(0, EV_MAX), bits);
  if (!test_bit (EV_ABS, bits))
    {
      return false;
    }
  return true;
}

void InputEventSystem::getTouchBounds(int fd)
{
  unsigned long bits[NBITS(KEY_MAX)];
  struct input_absinfo abs;

  ioctl (fd, EVIOCGBIT (EV_ABS, KEY_MAX), bits);
  if (!(test_bit (ABS_MT_POSITION_X, bits) &&
        test_bit (ABS_MT_POSITION_Y, bits)))
    {
      DOUT("absolute events");
      ioctl (fd, EVIOCGABS (ABS_X), &abs);
      minX  = abs.minimum;
      maxX  = abs.maximum;
      ioctl (fd, EVIOCGABS (ABS_Y), &abs);
      minY = abs.minimum;
      maxY = abs.maximum;
      DOUT("NO multitouch");
    }
  else
  {
    DOUT("absolute multitouch events");
    ioctl (fd, EVIOCGABS (ABS_MT_POSITION_X), &abs);
    minX = abs.minimum;
    maxX = abs.maximum;
    ioctl (fd, EVIOCGABS (ABS_MT_POSITION_Y), &abs);
    minY = abs.minimum;
    maxY = abs.maximum;
    DOUT("HAS multitouch");
  }
  dx = maxX - minX;
  dy = maxY - minY;    
}

InputEventSystem::InputEventSystem()
{
  minX = 0;
  maxX = 0;
  dx = 0;
  minY = 0;
  maxY = 0;
  dy = 0;
  currentX = 0;
  currentY = 0;

  lastX = 0;
  lastY = 0;
  resetLastUpdateTime();

  displayWidth = 1280;
  displayHeight = 800;
  
  currentState = 0;
}

const char* InputEventSystem::state2string()
{
  const char* result = "?";
  switch(currentState)
  {
    case ST_TOUCH_UPDATE:result="UPDATE";break;
    case ST_TOUCH_DOWN:result="DOWN";break;
    case ST_TOUCH_UP:result="UP";break;
  }
  return result;
}

void InputEventSystem::resetLastUpdateTime()
{
    lastUpdateTime.tv_sec = 0;
    lastUpdateTime.tv_usec = 0;
}

void InputEventSystem::parse(struct input_event* events, u32 num)
{
  currentState = ST_TOUCH_UPDATE;
  for(u32 i=0; i<num; ++i)
  {
    struct input_event* ev = &(events[i]);
//    DOUT("time: "<<(s64)ev->time.tv_sec<<" "<<(s64)ev->time.tv_usec);
    switch(ev->type)
    {
      case EV_ABS:
      {
        switch(ev->code)
        {
          case ABS_X:currentX = floorf((ev->value/dx)*displayWidth);break;
          case ABS_Y:currentY = floorf(displayHeight - (ev->value/dy)*displayHeight);break;
          default:
            //DOUT("tf");
//            DOUT(code2string(ev->type, ev->code, "?"));
            break;
        }
      }
      case EV_KEY:
      {
        switch(ev->code)
        {
          case BTN_TOUCH:
          {
            switch(ev->value)
            {
              case 0:currentState=ST_TOUCH_UP;break;
              case 1:currentState=ST_TOUCH_DOWN;break;
            }
          }
        }
      }
      case EV_SYN:
      {
        switch(ev->code)
        {
          case SYN_REPORT:
          {
            emitEvent(ev);
            break;
          }
        }
      }
    }
  }
}

bool tooFast(const struct timeval& t1, const struct timeval& t2)
{
  return (t1.tv_sec == t2.tv_sec) && ((t1.tv_usec/1000) == (t2.tv_usec/1000));
}

void InputEventSystem::emitEvent(struct input_event* ev)
{
  bool doEmit = false;

  if((currentState == ST_TOUCH_UPDATE) && (previousState == ST_TOUCH_UPDATE))
  {
//    DOUT("repeat update");
    if(tooFast(ev->time, lastUpdateTime))
    {
//      DOUT("too fast");
    }
    else if((floorf(currentX) == floorf(lastX)) && (floorf(currentY) == floorf(lastY)))
    {
//      DOUT("same coords");
    }
    else
    {
      doEmit = true;
      lastUpdateTime = ev->time;
      lastX = currentX;
      lastY = currentY;
    }
  }
  else
  {
//    DOUT("no repeat");
    doEmit = true;
    previousState = currentState;
  }

  if(doEmit)
  {
//    DOUT(state2string() << " " << (s32)currentX  << " " << (s32)currentY << " time: "<<(s64)ev->time.tv_sec<<" "<<(s64)ev->time.tv_usec);

    lost::Event* event = _appInstance->eventPool->borrowEvent();

    switch(currentState)
    {
      case ST_TOUCH_UP:
        event->base.type = lost::ET_MouseUp;
        event->mouseEvent.x = currentX;
        event->mouseEvent.y = currentY;       
        break; 
      case ST_TOUCH_DOWN:
        event->base.type = lost::ET_MouseDown;
        event->mouseEvent.x = currentX;
        event->mouseEvent.y = currentY;        
        break; 
      case ST_TOUCH_UPDATE:      
        event->base.type = lost::ET_MouseMove;
        event->mouseEvent.x = currentX;
        event->mouseEvent.y = currentY;        
        break;
    }

    _appInstance->eventQueue->addEventToNextQueue(event);    
  }
  else
  {
//    DOUT("dropping " << state2string());
  }
}

void InputEventSystem::run(const char* deviceName)
{
  struct input_event ev[64];
  int fd, rd, value, size = sizeof (struct input_event);
  char name[256] = "Unknown";

  if ((getuid ()) != 0)
  {
    DOUT("You are not root! This may not work...");
  }

  //Open Device
  if ((fd = open (deviceName, O_RDONLY)) == -1)
  {
    DOUT(deviceName << "is not a vaild device.");
    exit(1);
  }

  if(hasAbsolutEvents(fd))
  {
    DOUT("input device provides absolute events");
  }
  else
  {
    DOUT("input device does NOT provide absolute events");
  }

  getTouchBounds(fd);
  DOUT("touch bounds: " << minX << " " << maxX << " " << minY << " " << maxY);

  //Print Device Name
  ioctl (fd, EVIOCGNAME (sizeof (name)), name);
  DOUT ("Reading From : " << deviceName << "(" << name << ")");
  DOUT("reading , elem size: " << size << " max buffer: " << size << " " << size*64);
  while (1){
      if ((rd = read (fd, &ev, size*64 )) < size)
      {
        DOUT("read()");
        exit(0);
      }
      //printf("read bytes: %d elems: %d\n", rd, rd/size);
      u32 num = rd/size;
      parse(ev, num);
/*      value = ev[0].value;

      printf("..\n");
      for(int i=0; i<rd/size; ++i)
      {
        logEvent(&ev[i]);
      }*/

/*      if (value != ' ' && ev.value == 1 && ev.type == 1){ // Only read the key press event
       printf ("Code[%d]\n", (ev.code));
      }*/
  }  
}

}

