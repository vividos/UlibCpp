//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012,2017 Michael Fink
//
/// \file TestCommandLineParser.cpp tests for CommandLineParser class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/CommandLineParser.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests for CommandLineParser
   TEST_CLASS(TestCommandLineParser)
   {
      /// tests CommandLineParser, empty cmdline
      TEST_METHOD(TestParser1)
      {
         CommandLineParser parser(_T(""));

         CString c1;
         Assert::IsFalse(parser.GetNext(c1));
      }

      /// tests CommandLineParser, some words
      TEST_METHOD(TestParser2)
      {
         CommandLineParser parser(_T("abc def 123   xyz  a"));

         CString c1;
         Assert::IsTrue(parser.GetNext(c1)); Assert::IsTrue(c1 == _T("abc"));
         Assert::IsTrue(parser.GetNext(c1)); Assert::IsTrue(c1 == _T("def"));
         Assert::IsTrue(parser.GetNext(c1)); Assert::IsTrue(c1 == _T("123"));
         Assert::IsTrue(parser.GetNext(c1)); Assert::IsTrue(c1 == _T("xyz"));
         Assert::IsTrue(parser.GetNext(c1)); Assert::IsTrue(c1 == _T("a"));
         Assert::IsFalse(parser.GetNext(c1));
      }

      /// parses command line with argc and argv values
      TEST_METHOD(TestParserArgcArgv)
      {
         LPCTSTR argv[] =
         {
            _T("arg1"),
            _T("\"arg2"),
            _T("arg2\""),
         };

         int argc = sizeof(argv) / sizeof(*argv);
         CommandLineParser parser(argc, argv);

         CString c1;
         Assert::IsTrue(parser.GetNext(c1));
         Assert::IsTrue(c1 == argv[0]);

         Assert::IsTrue(parser.GetNext(c1));
         Assert::IsTrue(c1 == _T("arg2 arg2"));

         Assert::IsFalse(parser.GetNext(c1));
      }

      TEST_METHOD(TestParseParams)
      {
         CommandLineParser parser(_T("abc 123 /test qwerty --linux-help"));

         CString parameter;
         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("abc"), parameter);

         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("123"), parameter);

         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("/test"), parameter);

         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("qwerty"), parameter);

         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("--linux-help"), parameter);

         Assert::IsFalse(parser.GetNext(parameter));
      }

      TEST_METHOD(TestParseQuotedFilenames)
      {
         CommandLineParser parser(_T("\"some\" \"quoted\" text \"in cmdline\" "));

         CString parameter;
         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("some"), parameter);

         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("quoted"), parameter);

         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("text"), parameter);

         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("in cmdline"), parameter);

         Assert::IsFalse(parser.GetNext(parameter));
      }

      TEST_METHOD(TestParseQuotesInString)
      {
         CommandLineParser parser(_T("\"ab cd\" def \"asdf"));

         CString parameter;
         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("ab cd"), parameter);

         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("def"), parameter);

         Assert::IsTrue(parser.GetNext(parameter));
         Assert::AreEqual(_T("asdf"), parameter);

         Assert::IsFalse(parser.GetNext(parameter));
      }
   };

} // namespace UnitTest
