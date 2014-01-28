#pragma once

namespace lost
{
  void log(const string& msg);
}

#define LLOG(s) { \
lost::StringStream ss; \
ss << "(" << lost::Path(__FILE__).file() << " " << __FUNCTION__ << " " << __LINE__ << ") " << s; \
lost::log(ss.str()); \
};

#define DOUT(s) LLOG(s);
#define IOUT(s) LLOG("INFO: " << s);
#define WOUT(s) LLOG("WARNING: " << s);
#define EOUT(s) LLOG("ERROR: " << s);


#define ASSERT(cond, ms) { bool LOST_ASSERT_COND = (cond); if(!LOST_ASSERT_COND) {EOUT(ms << " " << string(#cond)); abort();} }

