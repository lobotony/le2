#pragma once

#include "lost/Application.h"

namespace lost
{

struct UiTestApp : Application
{
  UiTestApp();
  void startup();
  void update();
  void shutdown();
  
  bool first;
  bool logged;
  
  bool next;
  
  LayerPtr cursor;
  
  LabelPtr label;
  bool growing; 
};

}


