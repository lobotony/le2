#include "UserInterface.h"

namespace lost
{

UserInterface::UserInterface()
{
  rootView.reset(new View);
}

UserInterface::~UserInterface()
{
}

void UserInterface::update()
{
  // FIXME: event system needs to handle any incoming events
  // FIXME: layout system needs to layout any views and layers in queue
}

void UserInterface::draw(Context* glContext)
{
  renderSystem.draw(glContext, rootView);
}

}
