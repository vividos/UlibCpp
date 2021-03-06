//
// ulib - a collection of useful classes
// Copyright (C) 2008,2012,2014,2017,2020 Michael Fink
//
/// \file ErrorMessage.hpp Win32 error message
//
#pragma once

namespace Win32
{
   /// \brief Win32 API error message
   /// \details Formats a readable error message from an error value obtained by
   /// GetLastError() or other means.
   class ErrorMessage
   {
   public:
      /// ctor
      /// \param[in] errorCode win32 error code
      explicit ErrorMessage(DWORD errorCode = LastError())
         :m_errorCode(errorCode)
      {
      }

      /// Returns formatted error message
      CString ToString();

      /// Returns last error code
      static DWORD LastError();

   private:
      /// error code
      DWORD m_errorCode;
   };

} // namespace Win32
