//
// ulib - a collection of useful classes
// Copyright (C) 2004,2005,2017,2020 Michael Fink
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

      /// tests AsString() method with unavailable resource
      TEST_METHOD(ResourceAsString_UnavailableResource)
      {
         HINSTANCE instance = ::GetModuleHandle(_T("test.dll"));
         Win32::ResourceData resourceData(MAKEINTRESOURCE(128 + 64 + 42), _T("ABC123"), instance);

         Assert::IsFalse(resourceData.IsAvailable());

         CString text = resourceData.AsString();
         Assert::IsTrue(text.IsEmpty(), _T("text returned from AsString() must be empty"));
      }

      /// tests resource extraction as raw data
      TEST_METHOD(ResourceAsRawData)
      {
         // set up
         HINSTANCE instance = ::GetModuleHandle(_T("test.dll"));
         Win32::ResourceData resourceData(MAKEINTRESOURCE(IDR_RT_RCDATA_TEST), _T("RT_RCDATA"), instance);

         // run
         Assert::IsTrue(resourceData.IsAvailable());

         std::vector<BYTE> rawData;
         bool ret = resourceData.AsRawData(rawData);

         // check
         Assert::IsTrue(ret, L"AsRawData() must have returned true");
         Assert::IsFalse(rawData.empty(), _T("binary data returned from AsRawData() must not be empty"));
      }

      /// tests AsRawData() method with unavailable resource
      TEST_METHOD(ResourceAsRawData_UnavailableResource)
      {
         // set up
         HINSTANCE instance = ::GetModuleHandle(_T("test.dll"));
         Win32::ResourceData resourceData(MAKEINTRESOURCE(128 + 64 + 42), _T("ABC123"), instance);

         // run
         Assert::IsFalse(resourceData.IsAvailable());

         std::vector<BYTE> rawData;
         bool ret = resourceData.AsRawData(rawData);

         // check
         Assert::IsFalse(ret, L"AsRawData() must have returned false");
         Assert::IsTrue(rawData.empty(), _T("binary data returned from AsRawData() must be empty"));
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

         Assert::IsTrue(Path::FileExists(filename), _T("file must exist after call to AsFile()"));
      }
   };

} // namespace UnitTest
