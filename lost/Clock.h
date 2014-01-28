#pragma once

namespace lost
{
struct Clock
{
  Clock();
  
  void reset(); // resets startTime and lastUpdateTime to now.
  void update();
  
  double startTime; // absolute time value in seconds with which this clock was started and initialised
  double lastUpdateTime;   // time of last update call

  double deltaUpdate; // delta since last update call
  double deltaStart; // delta since last update call and start time
};
}


