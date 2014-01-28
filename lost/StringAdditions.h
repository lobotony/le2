#pragma once

namespace lost
{

u32 djb2Hash(const char* data);

bool replace(string& value,
             string const& search,
             string const& replace);


string trim(const string& pString,
            const string& pWhitespace = " \t");

void split(const string& str,
           vector<string>& tokens,
           const string& delimiters = " ",
           const bool trimEmpty = false);

//  "\r\n" and "\r" to "\n"
void normaliseLinebreaks(string& data);

u32string convertUtf8ToUtf32(const string& utf8string);
string convertUtf32ToUtf8String(const u32string& utf32string);


}



