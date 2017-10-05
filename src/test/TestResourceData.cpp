//
// ulib - a collection of useful classes
// Copyright (C) 2004,2005,2017 Michael Fink
//
/// \file TestResourceData.cpp unit test for class ResourceData
//
// Build Manager
// Copyright (C) 2004, 2005 Michael Fink
//

#include "stdafx.h"
#include <ulib/win32/ResourceData.hpp>
#include <ulib/unittest/AutoCleanupFile.hpp>
#include <ulib/Path.hpp>
#include "resource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   TEST_CLASS(TestResourceData)
   {
      /// tests resource extraction as string
      TEST_METHOD(ResourceAsString)
      {
         HINSTANCE instance = ::GetModuleHandle(_T("test.dll"));
         Win32::ResourceData resourceData(MAKEINTRESOURCE(IDR_RT_RCDATA_TEST), _T("RT_RCDATA"), instance);

         Assert::IsTrue(resourceData.IsAvailable());

         CString text = resourceData.AsString();
         Assert::IsFalse(text.IsEmpty(), _T("text returned from AsString() must not be empty"));
      }

      /// tests resource extraction as file
      TEST_METHOD(ResourceAsFile)
      {
         HINSTANCE instance = ::GetModuleHandle(_T("test.dll"));
         Win32::ResourceData resourceData(MAKEINTRESOURCE(IDR_RT_RCDATA_TEST), _T("RT_RCDATA"), instance);

         Assert::IsTrue(resourceData.IsAvailable());

         // generate filename
         CString filename = Path::Combine(Path::TempFolder(), _T("ResourceAsFile.txt"));
         ::DeleteFile(filename);

         // this object is to auto-cleanup the file in case of an assertion/exception
         UnitTest::AutoCleanupFile AutoCleanupFile(filename);

         Assert::IsTrue(resourceData.AsFile(filename));

         Assert::IsTrue(Path(filename).FileExists(), _T("file must exist after call to AsFile()"));
      }
   };

} // namespace UnitTest
