#ifndef GUIRO_UI_H
#define GUIRO_UI_H

#include "guiro/View.h"
#include "guiro/EventSystem.h"
#include "guiro/UpdateSystem.h"

namespace lost
{

struct UserInterface;
typedef lost::shared_ptr<UserInterface> UserInterfacePtr;

struct Context;

struct UserInterface : View
{
  UserInterface();
  ~UserInterface();
  
  void update();
  void draw(Context* glContext);
  
  EventSystem eventSystem;
  UpdateSystem updateSystem;
};
}

#endif
