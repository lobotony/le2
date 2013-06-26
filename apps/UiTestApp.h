#ifndef LOST_UITESTAPP_H
#define LOST_UITESTAPP_H

#include "lost/Application.h"

namespace lost
{

struct UiTestApp : Application
{
  void startup();
  void update();
  void shutdown();
  
  bool first;
  bool logged;
};

}

#endif

