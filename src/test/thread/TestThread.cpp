//
// ulib - a collection of useful classes
// Copyright (C) 2020 Michael Fink
//
/// \file TestThread.cpp unit test for Thread class
//
#include "stdafx.h"
#include <ulib/thread/Thread.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// Tests for Thread class
   TEST_CLASS(TestThread)
   {
      /// tests SetName() method
      TEST_METHOD(TestSetName)
      {
         try
         {
            Thread::SetName(L"thread-name1");
            Thread::SetName("thread-name2");
         }
         catch (...)
         {
            Assert::Fail(L"SetName() must not throw an exception");
         }
      }

      /// tests CurrentId() method
      TEST_METHOD(TestCurrentId)
      {
         Assert::AreNotEqual<DWORD>(0xFFFFFFFF, Thread::CurrentId(), L"current thread ID must not be -1");
      }
   };

} // namespace UnitTest
