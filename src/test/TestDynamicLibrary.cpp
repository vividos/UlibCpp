//
// ulib - a collection of useful classes
// Copyright (C) 2020 Michael Fink
//
/// \file TestDynamicLibrary.cpp unit test for class DynamicLibrary
//

#include "stdafx.h"
#include <ulib/DynamicLibrary.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   TEST_CLASS(TestDynamicLibrary)
   {
      /// tests resource extraction as string
      TEST_METHOD(TestLoadLibrary)
      {
         // set up
         DynamicLibrary lib{ _T("kernel32.dll") };

         // check
         Assert::IsTrue(lib.IsLoaded(), L"kernel32 library must have been loaded");

         Assert::IsFalse(lib.IsFunctionAvail("abc123"), L"unknown function must not be available");
         Assert::IsTrue(lib.IsFunctionAvail("GetModuleFileNameW"), L"well known function must be available");

         typedef void (*T_Abc123)();
         auto funcAbc123 = lib.GetFunction<T_Abc123>("abc123");
         Assert::IsNull(funcAbc123, L"getting unknown function must return NULL");

         typedef DWORD (WINAPI *T_GetModuleFileNameW)(HMODULE, LPWSTR, DWORD);
         T_GetModuleFileNameW funcGetModuleFileNameW = lib.GetFunction<T_GetModuleFileNameW>("GetModuleFileNameW");
         Assert::IsNotNull(funcGetModuleFileNameW, L"getting well known function must not return NULL");
      }
   };

} // namespace UnitTest
