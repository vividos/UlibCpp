//
// ulib - a collection of useful classes
// Copyright (C) 2000-2022 Michael Fink
//
/// \file UTF8.cpp UTF-8 conversion
//

#include "stdafx.h"
#include <ulib/UTF8.hpp>

#if __ANDROID__
#include <locale>
#include <codecvt>
#endif

void StringToUTF8(const CString& text, std::vector<char>& utf8Buffer)
{
#if WIN32
#if defined(UNICODE) || defined(_UNICODE)
   // calculate the bytes necessary
   unsigned int length = ::WideCharToMultiByte(CP_UTF8, 0, text, -1, nullptr, 0, nullptr, nullptr);

   utf8Buffer.resize(length);

   // convert
   ::WideCharToMultiByte(CP_UTF8, 0, text, -1, &utf8Buffer[0], length, nullptr, nullptr);
#else
#error non-unicode variant not implemented!
#endif
#endif

#if __ANDROID__
#if defined(UNICODE) || defined(_UNICODE)
   std::wstring source{ text.GetString() };

   std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
   std::string dest = convert.to_bytes(source);

   utf8Buffer.assign(dest.data(), dest.data() + dest.length());
#else
   utf8Buffer.assign(text.GetString(), text.GetString() + text.GetLength());
#endif
#endif
}

CString UTF8ToString(const char* utf8Text)
{
#if WIN32
   int bufferLength = MultiByteToWideChar(CP_UTF8, 0,
      utf8Text, -1,
      nullptr, 0);

   CString text;
   MultiByteToWideChar(CP_UTF8, 0,
      utf8Text, -1,
      text.GetBuffer(bufferLength), bufferLength);

   text.ReleaseBuffer();
   return text;
#endif

#if __ANDROID__
   std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
   std::wstring dest = convert.from_bytes(utf8Text);

   return CString{ dest.c_str() };
#endif
}
