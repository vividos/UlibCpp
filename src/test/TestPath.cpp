//
// ulib - a collection of useful classes
// Copyright (C) 2017 Michael Fink
//
/// \file TestPath.cpp Unit tests for Path class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/Path.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// Tests Path class
   TEST_CLASS(TestPath)
   {
   public:
      /// Tests default ctor
      TEST_METHOD(TestDefaultCtor)
      {
         Path p1, p2;
      }

      /// Tests ExtensionOnly() method
      TEST_METHOD(TestExtensionOnly)
      {
         Path p1(_T("filename.ext"));
         Path p2(_T("filename"));
         Path p3(_T("filename."));
         Path p4(_T("c:\\my.test\\filename"));

         Assert::AreEqual(_T(".ext"), p1.ExtensionOnly(), "extension must be .ext");
         Assert::AreEqual(_T(""), p2.ExtensionOnly(), "extension must be empty");
         Assert::AreEqual(_T("."), p3.ExtensionOnly(), "extension must be .");
         Assert::AreEqual(_T(""), p4.ExtensionOnly(), "extension must be empty");
      }

      /// Tests for GetCommonRootPath() method
      TEST_METHOD(TestGetCommonRootPath)
      {
         CString path1 = _T("C:\\win\\desktop\\temp.txt");
         CString path2 = _T("c:\\win\\tray\\sample.txt");
         CString path3 = _T("E:\\acme\\three");

         Assert::AreEqual(_T("C:\\win\\"), Path::GetCommonRootPath(path1, path2));
         Assert::AreEqual(_T(""), Path::GetCommonRootPath(path1, path3));
         Assert::AreEqual(_T(""), Path::GetCommonRootPath(path2, path3));

         Assert::AreEqual(_T("C:\\win\\desktop\\"), Path::GetCommonRootPath(path1, _T("C:\\win\\desktop\\")));
         Assert::AreEqual(_T("C:\\win\\desktop\\"), Path::GetCommonRootPath(path1, _T("C:\\win\\desktop")));

         Assert::AreEqual(_T("E:\\acme\\three\\"), Path::GetCommonRootPath(path3, path3));
      }
   };
}
