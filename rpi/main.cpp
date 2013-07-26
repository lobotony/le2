#include "lost/Runner.h"
#include "apps/DemoApp.h"
#include "apps/SunApp.h"
#include "apps/UiTestApp.h"
#include "apps/BezierEd.h"
#include "apps/AnimTest.h"

int main(int argn, char** argc)
{
//  lost::run(new lost::DemoApp);
//  lost::run(new lost::SunApp);
//  lost::run(new lost::UiTestApp);
  lost::run(new lost::BezierEd);
//  lost::run(new lost::AnimTest);
  return EXIT_SUCCESS;
}

