//
// ulib - a collection of useful classes
// Copyright (C) 2019 Michael Fink
//
/// \file TestSingleton.cpp tests for Singleton class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/Singleton.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// singleton test class
   class MySingleton : public Singleton<MySingleton>
   {
   public:
      int m_value;

      MySingleton()
         :m_value(42)
      {
      }
   };

   /// tests Singleton class
   TEST_CLASS(TestSingleton)
   {
   public:
      /// tests Instance() method
      TEST_METHOD(TestInstance)
      {
         Assert::AreEqual(42, MySingleton::Instance().m_value, _T("initial value must match"));

         MySingleton::Instance().m_value = 64;

         Assert::AreEqual(64, MySingleton::Instance().m_value, _T("newly set value must match"));
      }
   };

} // namespace UnitTest

IMPLEMENT_SINGLETON(UnitTest::MySingleton)
