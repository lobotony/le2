#include "lost/Clock.h"
#include "lost/Platform.h"

namespace lost
{

Clock::Clock()
{
  reset();
}

void Clock::reset()
{
  startTime = currentTimeSeconds();
  lastUpdateTime = startTime;
}

void Clock::update()
{
  double now = currentTimeSeconds();
  deltaUpdate = now - lastUpdateTime;
  deltaStart = now - startTime;
  lastUpdateTime = now;
}

}


