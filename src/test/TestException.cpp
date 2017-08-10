//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012,2017 Michael Fink
//
/// \file TestException.cpp tests for Exception class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Exception.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests Exception class
   TEST_CLASS(TestException)
   {
   public:
      /// Tests default ctor
      TEST_METHOD(TestDefaultCtor)
      {
         Exception e1(__FILE__, __LINE__);
         Exception e2(_T("my message"), __FILE__, __LINE__);
      }

#pragma warning(disable: 4702) // unreachable code
      /// tests throwing and catching by catch(...)
      TEST_METHOD(TestCatchingByCatchAll)
      {
         try
         {
            throw Exception(__FILE__, __LINE__);
            Assert::Fail(_T("must throw exception"));
         }
         catch (...)
         {
            return;
         }
      }
#pragma warning(default: 4702)

      /// tests if Exception is derived from std::exception
      TEST_METHOD(TestDerivedByStdException)
      {
         try
         {
            throw Exception(__FILE__, __LINE__);
         }
         catch (const std::exception& ex)
         {
            ex;
            // ok
         }
#pragma warning(disable: 4286) // 'const Exception &': is caught by base class ('const std::exception &') on line xx
         catch (const Exception& ex)
         {
            ex;
            Assert::Fail(_T("must be caught by std::exception catch clause"));
         }
         catch (...)
         {
            Assert::Fail(_T("must be caught by std::exception catch clause"));
         }
      }

      /// tests rethrowing exception
      TEST_METHOD(TestRethrowException)
      {
         try
         {
            try
            {
               throw Exception(__FILE__, __LINE__);
            }
            catch (const Exception& ex)
            {
               throw ex;
            }
         }
         catch (const Exception& ex)
         {
            ex;
            return;
         }
      }
   };

} // namespace UnitTest
