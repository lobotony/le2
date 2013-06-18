#include "lost/Runner.h"
#include "lost/DemoEngine.h"
#include "lost/SunEngine.h"

namespace lost
{

void run()
{
  run(new DemoEngine);
//  run(new SunEngine);
}

}
