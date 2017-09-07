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
   };
}
