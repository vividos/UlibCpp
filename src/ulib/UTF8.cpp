//
// ulib - a collection of useful classes
// Copyright (C) 2000-2017 Michael Fink
//
/// \file UTF8.cpp UTF-8 conversion
//

#include "stdafx.h"
#include "UTF8.hpp"

void StringToUTF8(const CString& text, std::vector<char>& utf8Buffer)
{
#if defined(UNICODE) || defined(_UNICODE)
   // calculate the bytes necessary
   unsigned int length = ::WideCharToMultiByte(CP_UTF8, 0, text, -1, nullptr, 0, nullptr, nullptr);

   utf8Buffer.resize(length);

   // convert
   ::WideCharToMultiByte(CP_UTF8, 0, text, -1, &utf8Buffer[0], length, nullptr, nullptr);
#else
#error non-unicode variant not implemented!
#endif
}

CString UTF8ToString(const char* utf8Text)
{
   int bufferLength = MultiByteToWideChar(CP_UTF8, 0,
      utf8Text, -1,
      nullptr, 0);

   CString text;
   MultiByteToWideChar(CP_UTF8, 0,
      utf8Text, -1,
      text.GetBuffer(bufferLength), bufferLength);

   text.ReleaseBuffer();
   return text;
}
