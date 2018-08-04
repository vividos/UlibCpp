//
// ulib - a collection of useful classes
// Copyright (C) 2018 Michael Fink
//
/// \file TestConfog.cpp tests for config headers
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/config/Common.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests for config headers
   TEST_CLASS(TestConfig)
   {
      /// tests sizeof_array
      TEST_METHOD(TestSizeofArray)
      {
         int a[14], b[1];

         Assert::AreEqual<unsigned int>(14, sizeof_array(a), _T("size of array must match"));
         Assert::AreEqual<unsigned int>(1, sizeof_array(b), _T("size of array must match"));
      }
   };

} // namespace UnitTest
