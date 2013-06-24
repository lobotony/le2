#include "lost/StringAdditions.h"
#include <string>
#include <codecvt>
#include <iostream>
#include "thirdparty/utf8.h"

namespace lost
{

using namespace std;

bool replace(string& value,
             string const& search,
             string const& replace)
{
  bool replaced = false;
  std::string::size_type  next;

  for(next = value.find(search);        // Try and find the first match
      next != std::string::npos;        // next is npos if nothing was found
      next = value.find(search,next)    // search for the next match starting after
                                        // the last match that was found.
     )
  {
    // Inside the loop. So we found a match.
    value.replace(next,search.length(),replace);   // Do the replacement.
    next += replace.length();                      // Move to just after the replace
                                                   // This is the point were we start
                                                   // the next search from.
    replaced = true;
  }
  return replaced;
}


string trim(const string& pString,
            const string& pWhitespace)
{
  const size_t beginStr = pString.find_first_not_of(pWhitespace);
  if (beginStr == std::string::npos)
  {
    // no content
    return "";
  }

  const size_t endStr = pString.find_last_not_of(pWhitespace);
  const size_t range = endStr - beginStr + 1;

  return pString.substr(beginStr, range);
}

void split(const std::string& str,
           vector<string>& tokens,
           const std::string& delimiters,
           const bool trimEmpty)
{
   std::string::size_type pos, lastPos = 0;
   while(true)
   {
      pos = str.find_first_of(delimiters, lastPos);
      if(pos == std::string::npos)
      {
         pos = str.length();

         if(pos != lastPos || !trimEmpty)
            tokens.push_back(string(str.data()+lastPos, pos-lastPos ));

         break;
      }
      else
      {
         if(pos != lastPos || !trimEmpty)
            tokens.push_back(string(str.data()+lastPos, pos-lastPos ));
      }

      lastPos = pos + 1;
   }
};

void normaliseLinebreaks(string& data)
{
  replace(data, "\r\n", "\n");
  replace(data, "\r", "\n");
}

u32string convertUtf8ToUtf32(const string& utf8string)
{
  u32string result;
  utf8::utf8to32(utf8string.begin(), utf8string.end(), back_inserter(result));
  return result;
}

string convertUtf32ToUtf8String(const u32string& utf32string)
{
  string result;
  utf8::utf32to8(utf32string.begin(), utf32string.begin(), back_inserter(result));
  return result;
}


/*u32string convertUtf8ToUtf32(const string& utf8string)
{
  wstring_convert<codecvt_utf8<char32_t>, char32_t> convert;
  return convert.from_bytes(utf8string);
}

string convertUtf32ToUtf8String(const u32string& utf32string)
{
  wstring_convert<codecvt_utf8<char32_t>, char32_t> convert;
  return convert.to_bytes(utf32string);
}*/

}

