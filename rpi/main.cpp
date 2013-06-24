#include "lost/Runner.h"
#include "apps/DemoApp.h"
#include "apps/SunApp.h"
#include "apps/UiTestApp.h"

int main(int argn, char** argc)
{
//  lost::run(new lost::DemoApp);
//  lost::run(new lost::SunApp);
  lost::run(new lost::UiTestApp);
  return EXIT_SUCCESS;
}