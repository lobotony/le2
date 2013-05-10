#include "guiro/Frame.h"

namespace lost
{
Frame::Frame()
{
  clear();
}

void Frame::clear()
{
  x.clear();
  y.clear();
  w.clear();
  h.clear();
}


}
