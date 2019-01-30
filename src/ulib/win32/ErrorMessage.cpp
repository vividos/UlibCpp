//
// ulib - a collection of useful classes
// Copyright (C) 2008,2012,2014,2017,2019 Michael Fink
//
/// \file ErrorMessage.cpp Win32 error message
//

#include "stdafx.h"
#include <ulib/win32/ErrorMessage.hpp>

#ifdef __ANDROID__
#include <cerrno>
#endif

CString Win32::ErrorMessage::ToString()
{
#ifdef WIN32
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
#endif

#ifdef __ANDROID__
   CString errorMessage;

   errorMessage.Format(_T("Error %i (%hs)"),
      static_cast<int>(m_errorCode),
      strerror(static_cast<int>(m_errorCode)));
#endif

   return errorMessage;
}

DWORD Win32::ErrorMessage::LastError()
{
#ifdef WIN32
   return GetLastError();
#endif

#ifdef __ANDROID__
   return static_cast<DWORD>(errno);
#endif
}
