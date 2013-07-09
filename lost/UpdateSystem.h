#ifndef GUIRO_UPDATESYSTEM_H
#define GUIRO_UPDATESYSTEM_H

#include "guiro/types.h"

namespace lost
{
struct UpdateSystem
{
  UpdateSystem();
  ~UpdateSystem();
  
  void update(); // performs the actual updates in the correct order
};
}

#endif

