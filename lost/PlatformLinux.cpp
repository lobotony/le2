/*
Copyright (c) 2012 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// platform specific code, forwarded in Platform.h
// LINUX

#include "lost/Platform.h"
#include <string>
#include <stdexcept>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include <sys/prctl.h>

using namespace std;

namespace lost
{

  double currentTimeSeconds()
  {
    return ((double) currentTimeMicroSeconds() / 1000000.0);
  }

	// returns current time as string (e.g. "2007/11/26 23:30:37")
	string currentTimeFormat()
	{
	  char   timeformat[20];
	  time_t timestamp;

	  timestamp = time( &timestamp );
	  strftime(timeformat, 20, "%Y/%m/%d %H:%M:%S", localtime( &timestamp ));
	  return timeformat;
	}

	// returns current time in microseconds
	long currentTimeMicroSeconds()
	{
	  struct timeval tv;

	  gettimeofday(&tv, NULL);
	  return tv.tv_sec*1000000 + tv.tv_usec;
	}

	Path getApplicationFilename(bool excludeExtension = false)
	{
	  Path path("/proc");
	  struct stat info;
	  if (0 != stat(path.string().c_str(), &info) || !S_ISDIR(info.st_mode)) {
	    // There's no /proc filesystem, we can't find out a lot about our
	    // application.
	    ASSERT(false, "Could not find /proc filesystem!");
	  }


	  // Read the exe link in the /proc filesystem
	  std::ostringstream os;
	  os << getpid();
	  path /= os.str().c_str();
	  path /= "exe";

	  // There's no limit to how long a path in Linux can be, but let's assume
	  // it won't exceed 2k characters.
	  char pathbuf[2048];

	  ssize_t pathsize = readlink(path.string().c_str(), pathbuf,
	      sizeof(pathbuf));

	  if (-1 == pathsize) {
	    ASSERT(false, "Could not determine application path!");
	  }

	  return string(pathbuf, pathsize);
	}

	Path getApplicationDirectory()
	{
	  return getApplicationFilename().directory();
	}

	Path buildResourcePath( const string& filename )
	{
	  Path result = getResourcePath() / filename;

	  FILE *l_filecheck = fopen(result.string().c_str(), "r");
	  if (l_filecheck != NULL) fclose(l_filecheck);
	    else ASSERT(false, string("Couldn't find resource '"+ result.string() +"', does it exist in your resources directory? Is the spelling correct?").c_str());

	  return result;
	}

	Path getResourcePath()
	{
	  return getApplicationDirectory();
	}

	Path getUserDataPath() {
	  Path result;
	#ifdef ANDROID
	  struct passwd* pwdptr = getpwuid(getuid());
	  if (pwdptr != NULL) {
	result = pwdptr->pw_dir;
	  }
	#else
	  struct passwd pd;
	  struct passwd* pwdptr=&pd;
	  struct passwd* tempPwdPtr;
	  char pwdbuffer[200];
	  int  pwdlinelen = sizeof(pwdbuffer);

	  if ((getpwuid_r(getuid(), pwdptr, pwdbuffer, pwdlinelen, &tempPwdPtr)) == 0) {
	result = pd.pw_dir;
	  }
	#endif
	  return result;
	}

	Path buildUserDataPath( const string& filename )
	{
	  Path result = getUserDataPath() / filename;

	  FILE *l_filecheck = fopen(result.string().c_str(), "r");
	  if (l_filecheck != NULL) fclose(l_filecheck);
	    else ASSERT(false, string("Couldn't find resource '"+ result.string() +"', does it exist in your resources directory? Is the spelling correct?").c_str());

	  return result;
	}

	void setThreadName(const string& name) {
	  prctl(PR_SET_NAME, name.c_str(), 0, 0, 0);
	}

	string getThreadName() {
	  char buf[17];
	  buf[16] = 0;
	  prctl(PR_GET_NAME, buf, 0, 0, 0);
	  return buf;
	}

	bool setClipboardString(string const & str)
	{
	  // TODO
	  ASSERT(false, "not implemented");
	  return false;
	}

	string getClipboardString()
	{
	  // TODO
	  ASSERT(false, "not implemented");
	  return "";
	}
}
