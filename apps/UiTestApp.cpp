#include "apps/UiTestApp.h"
#include "lost/Context.h"

namespace lost
{

void UiTestApp::startup()
{
}

void UiTestApp::update()
{
  glContext->clearColor(Color(0,0,0,1));
  glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
}

void UiTestApp::shutdown()
{
}

}
