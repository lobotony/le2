#include "lost/Runner.h"
#include "apps/SunApp.h"
#include "apps/DemoApp.h"
#include "apps/UiTestApp.h"

int main(int argc, char *argv[])
{
//  lost::run(new lost::DemoApp);
//  lost::run(new lost::SunApp);
  lost::run(new lost::UiTestApp);
  return EXIT_SUCCESS;
}
