//
// ulib - a collection of useful classes
// Copyright (C) 2017 Michael Fink
//
/// \file TestPath.cpp Unit tests for Path class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Path.hpp"

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

   };
}
