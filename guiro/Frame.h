#ifndef GUIRO_FRAME_H
#define GUIRO_FRAME_H

#include "lost/Rect.h"

namespace lost
{
struct Frame
{
  Frame();
  ~Frame();
  
  Rect rect(Rect superRect);
};
}

#endif

