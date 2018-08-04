//
// ulib - a collection of useful classes
// Copyright (C) 2008,2012,2014,2017 Michael Fink
//
/// \file ErrorMessage.cpp Win32 error message
//

#include "stdafx.h"
#include <ulib/win32/ErrorMessage.hpp>

CString Win32::ErrorMessage::ToString()
{
   LPVOID messageBuffer = nullptr;
   ::FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      m_errorCode,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
      reinterpret_cast<LPTSTR>(&messageBuffer), 0, nullptr);

   CString errorMessage;
   if (messageBuffer)
   {
      errorMessage = reinterpret_cast<LPTSTR>(messageBuffer);
      LocalFree(messageBuffer);
   }

   errorMessage.TrimRight(_T("\r\n"));

   return errorMessage;
}

DWORD Win32::ErrorMessage::LastError() throw()
{
   return GetLastError();
}
