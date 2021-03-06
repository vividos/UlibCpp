//
// ulib - a collection of useful classes
// Copyright (C) 2007,2008,2009,2017,2020 Michael Fink
//
/// \file SystemException.hpp Win32 system exception class
//
#pragma once

#include "Exception.hpp"
#include "win32/ErrorMessage.hpp"

/// Win32 system exception
class SystemException : public Exception
{
public:
   /// ctor
   SystemException(const CString& message, DWORD win32ErrorCode, LPCSTR sourceFile, UINT sourceLine)
      :Exception(message + _T(": ") + Win32::ErrorMessage(win32ErrorCode).ToString(), sourceFile, sourceLine),
      m_win32ErrorCode(win32ErrorCode)
   {
   }

   /// returns Win32 error code
   DWORD Win32Error() const { return m_win32ErrorCode; }

private:
   /// Win32 error code
   DWORD m_win32ErrorCode;
};
