#include "lost/Runner.h"
#include "apps/SunApp.h"
#include "apps/DemoApp.h"
#include "apps/UiTestApp.h"
#include "apps/BezierEd.h"
#include "apps/AnimTest.h"
#include "apps/RpiDemo.h"

int main(int argc, char *argv[])
{
//  lost::run(new lost::DemoApp);
//  lost::run(new lost::SunApp);
//  lost::run(new lost::UiTestApp);
  lost::run(new lost::RpiDemo);
//  lost::run(new lost::BezierEd);

//  lost::run(new lost::AnimTest);

  return EXIT_SUCCESS;
}
