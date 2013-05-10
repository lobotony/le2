#ifndef GUIRO_FRAME_H
#define GUIRO_FRAME_H

#include "lost/Rect.h"

namespace lost
{

struct Frame
{
  Frame();
  
  void clear();
  
  struct Param
  {
    f32 abs;
    f32 rel;
    u32 kind;
    
    Param()
    {
      clear();
    }
    
    void clear()
    {
      set(0, 0, 0);
    }
    
    void set(f32 a, f32 r, u32 k)
    {
      abs = a;rel=r;kind=k;
    }
  };
  
  Param x;
  Param y;
  Param w;
  Param h;
};
}

#endif

