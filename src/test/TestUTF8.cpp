//
// ulib - a collection of useful classes
// Copyright (C) 2020 Michael Fink
//
/// \file TestUTF8.cpp unit test for UTF8 functions
//
#include "stdafx.h"
#include <ulib/UTF8.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   TEST_CLASS(TestUTF8)
   {
      /// tests resource extraction as string
      TEST_METHOD(TestStringToUTF8)
      {
         // U+20ac EURO SIGN and U+FEFF BYTE ORDER MARK
         CString text = L"\u20ac\uFEFFabc123";

         // run
         std::vector<char> buffer;
         StringToUTF8(text, buffer);

         // check
         std::string utf8text{ buffer.data() }; // buffer is zero terminated
         Assert::AreEqual<std::string>("\xe2\x82\xac\xEF\xBB\xBF""abc123", utf8text,
            L"Unicode text must have been converted correctly");
      }

      /// tests UTF8ToString() method
      TEST_METHOD(TestUTF8ToString)
      {
         // run
         CString text = UTF8ToString("\xe2\x82\xac\xEF\xBB\xBF""abc123");

         // check
         Assert::AreEqual(_T("\u20ac\uFEFFabc123"), text,
            L"UTF8-8 text must have been converted correctly");
      }
   };

} // namespace UnitTest
