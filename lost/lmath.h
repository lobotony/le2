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

// don't call this header "Math.h" or it might conflict with standard library "math.h" depending 
// on the order of includes in your current build setting and result in confusing errors.

namespace lost
{

  Vec2 catmullRomInterpolate(f32 t, const Vec2& cp0, const Vec2& cp1, const Vec2& cp2, const Vec2& cp3);

  Vec2 bezierInterpolate(f32 t, const Vec2& cp0, const Vec2& cp1, const Vec2& cp2, const Vec2& cp3);

  void catmullRomSegment(vector<Vec2>&        interpolated, // receives the interpolated points
                           uint32_t             pointOffset,  // offset write position into interpolated points. The current segment points will be written at pointOffset onwards
                           uint32_t             numPoints,    // number of points for this segment
                           const vector<Vec2>&  cp,           // control points (all of them)
                           uint32_t             cpoffset);     // offset into control points. Four points after and including cp[0+cpoffset] will be used

  void catmullRom(const vector<Vec2>& controlPoints,  // arbitrary number of control point, at least 4
                  u32 numInterpolatedPoints,          // number of points the result will contain
                  vector<Vec2>& interpolatedPoints);   // result vector, will be resized to numInterpolatedPoints

  void calculateNormals(const vector<Vec2>& points, // vector of points that form a line
                        vector<Vec2>& normals);     //receives normal vectors calculated from points

#if defined WIN32
    inline float fmaxf(const float& in1, const float& in2)
    {
      return (in1 > in2) ? in1 : in2;
    }
#endif
	  inline float max(const lost::shared_array<float> inArray, int inLength)
    {
      float result = (inLength > 0) ? inArray[0] : 0;
      for (int idx = 1; idx < inLength; ++idx)
        result = fmaxf(result, inArray[idx]);
      return result;
    }
    
    /* PI */
#ifndef M_PI
#define M_PI (float)3.1415926535897932384626433832795
#endif
    
    inline float deg2rad( float deg )
    {
      return deg / 360.0f * (2.0f * M_PI);
    }
    
    inline float rad2deg( float rad )
    {
      return (rad / (2.0f * M_PI)) * 360.0f;
    }

    // compares two floats, returning true if they are "near enough", i.e. their difference is within
    // the range of the provided epsilon value.
    // this is necessary to compensate for floating point rounding errors that occur with 32bit floats.
    inline bool compare(float a, float b, float epsilon)
    {
      float d = fabs(a-b);
      if(d <= epsilon)
        return true;
      else
        return false;
    }

    // calculate the next value that is >= inVal and also a power of two  
    static inline uint32_t nextPowerOf2(uint32_t inVal)
    {
      uint32_t result = 1;

      while(result < inVal)
      {
        result <<= 1;
      }

      return result;
    }
}

