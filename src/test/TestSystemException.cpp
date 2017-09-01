//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012,2017 Michael Fink
//
/// \file TestSystemException.cpp tests for SystemException class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/SystemException.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests SystemException class
   TEST_CLASS(TestSystemException)
   {
   public:
      /// tests formatting message
      TEST_METHOD(TestFormatMessage)
      {
         DWORD win32ErrorCode = 2;
         SystemException ex(_T("my message"), win32ErrorCode, __FILE__, __LINE__);

         CString message = ex.Message();
         Assert::IsFalse(message.IsEmpty(), _T("message must not be empty"));

         Assert::AreEqual(win32ErrorCode, ex.Win32Error(), _T("Win32 error must match"));
      }
   };

} // namespace UnitTest
