//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2020 Michael Fink
//
/// \file TestEndianAwareFilter.cpp unit tests for EndianAwareFilter
//

#include "stdafx.h"
#include <ulib/stream/EndianAwareFilter.hpp>
#include <ulib/stream/MemoryReadStream.hpp>
#include <ulib/stream/MemoryStream.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// 32-bit constant to read/write
   static const DWORD g_const32bit = 0x12345678;
   /// 16-bit constant to read/write
   static const WORD g_const16bit = 0x9abc;

   /// test result data
   static const BYTE g_testData[] =
   {
      0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, // BE dword, word
      0x78, 0x56, 0x34, 0x12, 0xbc, 0x9a, // LE dword, word
   };

   /// tests EndianAwareFilter class
   TEST_CLASS(TestEndianAwareFilter)
   {
      /// tests reading
      TEST_METHOD(TestRead)
      {
         // set up
         Stream::MemoryReadStream stream{ g_testData, sizeof_array(g_testData) };
         Stream::EndianAwareFilter filter{ stream };

         // run + check
         Assert::AreEqual(g_const32bit, filter.Read32BE(), L"read 32-bit word must match");
         Assert::AreEqual<DWORD>(g_const16bit, filter.Read16BE(), L"read 16-bit word must match");

         Assert::AreEqual(g_const32bit, filter.Read32LE(), L"read 32-bit word must match");
         Assert::AreEqual<DWORD>(g_const16bit, filter.Read16LE(), L"read 16-bit word must match");
      }

      /// tests writing
      TEST_METHOD(TestWrite)
      {
         // set up
         Stream::MemoryStream stream;
         Stream::EndianAwareFilter filter{ stream };

         // run
         filter.Write32BE(g_const32bit);
         filter.Write16BE(g_const16bit);
         filter.Write32LE(g_const32bit);
         filter.Write16LE(g_const16bit);

         // check
         const std::vector<BYTE>& resultData = stream.GetData();

         size_t length = sizeof_array(g_testData);
         Assert::AreEqual(length, resultData.size(), L"written size must match");
         Assert::IsTrue(0 == memcmp(&resultData[0], g_testData, length), L"result data must match test data");
      }
   };

} // namespace UnitTest
