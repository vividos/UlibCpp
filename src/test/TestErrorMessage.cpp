//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2017 Michael Fink
//
/// \file TestErrorMessage.cpp tests ErrorMessage class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/win32/ErrorMessage.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests Win32::ErrorMessage class
   TEST_CLASS(TestErrorMessage)
   {
   public:
      /// tests formatting error
      TEST_METHOD(TestFormatError)
      {
         DWORD win32ErrorCode = ERROR_FILE_NOT_FOUND;
         Win32::ErrorMessage  message(win32ErrorCode);

         Assert::IsFalse(message.ToString().IsEmpty(), _T("message must not be empty"));
      }
   };

} // namespace UnitTest
