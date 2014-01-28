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

#pragma once

// TargetConditionals defines the IPHONE Macros we require
// this header is only present if we build on an apple platform
#if defined __APPLE__
  #include <TargetConditionals.h>
#endif

#ifndef TARGET_OS_IPHONE
  #define TARGET_OS_IPHONE 0
#endif
#ifndef TARGET_IPHONE_SIMULATOR
  #define TARGET_IPHONE_SIMULATOR 0
#endif

#if defined WIN32
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>

  #define sleep Sleep

  #ifndef __FUNCTION__
    #ifdef __func__
      #define __FUNCTION__ __func__
    #else
      #define __FUNCTION__ "unknown"
    #endif
  #endif

  #ifndef __PRETTY_FUNCTION__
    #ifdef __FUNCDNAME__
      #define __PRETTY_FUNCTION__ __FUNCDNAME__
    #else
      #define __PRETTY_FUNCTION__ "unknown"
    #endif
  #endif
#endif

namespace lost
{
  typedef enum
  {
    platform_windows = 0,
    platform_mac     = 1,
    platform_linux   = 2,
    platform_iphone  = 3,
    platform_android = 4,
  } PlatformType;

  PlatformType getPlatform();

  bool isWindows();
  bool isMac();
  bool isLinux();
  bool isIPhone();

  typedef enum
  {
    endBigEndian    = 0,
    endLittleEndian = 1
  } Endianness;

  Endianness getEndianness();

  bool isBigEndian();
  bool isLittleEndian();
}

