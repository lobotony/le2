#ifndef LOST_UPDATESYSTEM_H
#define LOST_UPDATESYSTEM_H

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

