#include "lost/Log.h"
#include <iostream>

#ifdef LOST_PLATFORM_RPI
#include <syslog.h>
#endif


namespace lost
{

void log(const string& msg)
{
  std::cout << msg.c_str() << std::endl;
#ifdef LOST_PLATFORM_RPI
  syslog(LOG_MAKEPRI(LOG_USER, LOG_DEBUG), msg.c_str());
#endif
}

}