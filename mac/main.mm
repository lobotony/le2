#include "lost/Runner.h"
#include "apps/SunEngine.h"
#include "apps/DemoEngine.h"

int main(int argc, char *argv[])
{
//  lost::run(new lost::DemoEngine);
  lost::run(new lost::SunEngine);
  return EXIT_SUCCESS;
}
