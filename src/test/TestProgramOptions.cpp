//
// ulib - a collection of useful classes
// Copyright (C) 2020 Michael Fink
//
/// \file TestProgramOptions.cpp tests for ProgramOptions class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/ProgramOptions.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests for ProgramOptions class
   TEST_CLASS(TestProgramOptions)
   {
      /// tests custom RegisterOutputHandler()
      TEST_METHOD(TestCustomOutputHandler)
      {
         // set up
         ProgramOptions options;

         // run
         bool outputHandlerCalled = false;
         options.RegisterOutputHandler([&outputHandlerCalled](auto text) { outputHandlerCalled = true; });
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"), []() { return true; });
         options.OutputHelp();

         // check
         Assert::IsTrue(outputHandlerCalled, L"output handler must have been called");
      }

      /// tests OutputConsole() as output handler
      TEST_METHOD(TestOutputConsoleHandler)
      {
         ProgramOptions options;

         // run
         options.RegisterOutputHandler(&ProgramOptions::OutputConsole);
         options.OutputHelp();
      }

      /// tests RegisterHelpOption() method
      TEST_METHOD(TestRegisterHelpOption)
      {
         // set up
         ProgramOptions options1, options2, options3, options4;
         options1.RegisterHelpOption();
         options3.RegisterHelpOption();

         // run
         LPCTSTR commandLineWithoutHelp = _T("dummy.exe --dummy-switch");
         LPCTSTR commandLineWithHelp = _T("dummy.exe --help");

         options1.Parse(commandLineWithoutHelp);
         options2.Parse(commandLineWithoutHelp);
         options3.Parse(commandLineWithHelp);
         options4.Parse(commandLineWithHelp);

         // check
         Assert::IsFalse(options1.IsSelectedHelpOption(), L"no help option was passed");
         Assert::IsFalse(options2.IsSelectedHelpOption(), L"no help option was passed");
         Assert::IsTrue(options3.IsSelectedHelpOption(), L"help option must have been recognized");
         Assert::IsFalse(options4.IsSelectedHelpOption(), L"no help option was recognized");
      }

      /// tests Parse() method using argc/argv parameters
      TEST_METHOD(TestParseArgcArgv)
      {
         // set up
         ProgramOptions options;

         bool paramHandlerCalled = false;
         options.RegisterParameterHandler(
            [&paramHandlerCalled](const CString& param)
            {
               paramHandlerCalled = param == "clone";
               return paramHandlerCalled;
            });

         CString foundArgument;
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"),
            foundArgument);

         // run
         _TCHAR* args[] =
         {
            _T("dummy.exe"),
            _T("clone"),
            _T("--opt"),
            _T("\"abc 123\""),
         };

         options.Parse(static_cast<int>(sizeof_array(args)), args);

         // check
         Assert::IsTrue(paramHandlerCalled, L"parameter handler must have been called");
         Assert::AreEqual(_T("abc 123"), foundArgument, L"argument must match");
      }

      /// tests RegisterParameterHandler()
      TEST_METHOD(TestRegisterParameterHandler)
      {
         // set up
         ProgramOptions options;

         CString filename;
         bool paramHandlerCalled = false;
         options.RegisterParameterHandler(
            [&paramHandlerCalled, &filename](const CString& param)
            {
               paramHandlerCalled = true;
               filename = param;
               return true;
            });

         // run
         options.Parse(_T("dummy.exe --opt1 \"the filename.txt\" --opt2"));

         // check
         Assert::IsTrue(paramHandlerCalled, L"parameter handler must have been called");
         Assert::AreEqual(_T("the filename.txt"), filename, L"parsed parameter must match");
      }

      /// tests RegisterOption() method, single option without arguments
      TEST_METHOD(TestRegisterOption_OptionWithoutArgs)
      {
         // set up
         ProgramOptions options;

         bool handledOption = false;
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"),
            [&handledOption]()
            {
               handledOption = true;
               return true;
            });

         // run
         options.Parse(_T("dummy.exe --opt"));

         // check
         Assert::IsTrue(handledOption, L"option must have been handled");
      }

      /// tests RegisterOption() method, single option with single argument
      TEST_METHOD(TestRegisterOption_OptionSingleArgument)
      {
         // set up
         ProgramOptions options;

         bool handledOption = false;
         CString foundArgument;
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"),
            [&handledOption, &foundArgument](const CString& argument)
            {
               handledOption = true;
               foundArgument = argument;
               return true;
            });

         // run
         options.Parse(_T("dummy.exe --opt arg123"));

         // check
         Assert::IsTrue(handledOption, L"option must have been handled");
         Assert::AreEqual(_T("arg123"), foundArgument, L"argument must match");
      }

      /// tests RegisterOption() method, single option with multiple arguments
      TEST_METHOD(TestRegisterOption_OptionWithMultipleArgs)
      {
         // set up
         ProgramOptions options;

         bool handledOption = false;
         std::vector<CString> foundArgumentList;
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"),
            3,
            [&handledOption, &foundArgumentList](const std::vector<CString>& argumentList)
            {
               handledOption = true;
               foundArgumentList = argumentList;
               return true;
            });

         // run
         options.Parse(_T("dummy.exe --opt abc \"123 456\" xyz"));

         // check
         Assert::IsTrue(handledOption, L"option must have been handled");
         Assert::AreEqual<size_t>(3, foundArgumentList.size(), L"number of arguments must match");
         Assert::AreEqual(_T("abc"), foundArgumentList[0], L"argument must match");
         Assert::AreEqual(_T("123 456"), foundArgumentList[1], L"argument must match");
         Assert::AreEqual(_T("xyz"), foundArgumentList[2], L"argument must match");
      }

      /// tests RegisterOption() method, single boolean option
      TEST_METHOD(TestRegisterOption_BooleanOption)
      {
         // set up
         ProgramOptions options;

         bool handledOption = false;
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"),
            handledOption);

         // run
         options.Parse(_T("dummy.exe --opt"));

         // check
         Assert::IsTrue(handledOption, L"option must have been handled");
      }

      /// tests RegisterOption() method, single string option
      TEST_METHOD(TestRegisterOption_StringOption)
      {
         // set up
         ProgramOptions options;

         CString foundArgument;
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"),
            foundArgument);

         // run
         options.Parse(_T("dummy.exe --opt \"abc 123\""));

         // check
         Assert::AreEqual(_T("abc 123"), foundArgument, L"argument must match");
      }

      /// tests RegisterOption() method, parsing option's short name
      TEST_METHOD(TestRegisterOption_ShortName)
      {
         // set up
         ProgramOptions options;

         bool handledOption = false;
         options.RegisterOption(_T("i"), _T("ignore"), _T("Ignore option"),
            handledOption);
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"),
            handledOption);

         // run
         options.Parse(_T("dummy.exe -o"));

         // check
         Assert::IsTrue(handledOption, L"option must have been handled");
      }

      /// tests RegisterOption() method, parsing an unknown option
      TEST_METHOD(TestRegisterOption_ErrorUnknownOption)
      {
         // set up
         ProgramOptions options;
         options.RegisterOutputHandler(&ProgramOptions::OutputConsole);

         bool handledOption = false;
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"),
            handledOption);

         // run
         options.Parse(_T("dummy.exe --optaaargh"));

         // check
         Assert::IsFalse(handledOption, L"option must not have been handled");
      }

      /// tests RegisterOption() method, passing an invalid number of arguments
      TEST_METHOD(TestRegisterOption_ErrorInvalidNumberOfArgs)
      {
         // set up
         ProgramOptions options;
         options.RegisterOutputHandler(&ProgramOptions::OutputConsole);

         bool handledOption = false;
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"),
            3,
            [&handledOption](const std::vector<CString>& argumentList)
            {
               handledOption = argumentList.size() == 3;
               return handledOption;
            });

         // run
         options.Parse(_T("dummy.exe --opt abc \"123 456\""));

         // check
         Assert::IsFalse(handledOption, L"option must not have been handled");
      }

      /// tests RegisterOption() method, single option with single invalid argument
      TEST_METHOD(TestRegisterOption_ErrorInvalidSingleArgument)
      {
         // set up
         ProgramOptions options;
         options.RegisterOutputHandler(&ProgramOptions::OutputConsole);

         bool handledOption = false;
         options.RegisterOption(_T("o"), _T("opt"), _T("Option to test"),
            [&handledOption](const CString& argument)
            {
               handledOption = argument == "arg123";
               return handledOption;
            });

         // run
         options.Parse(_T("dummy.exe --opt hello"));

         // check
         Assert::IsFalse(handledOption, L"option must not have been handled");
      }

      /// tests RegisterParameterHandler(), with an invalid parameter command
      TEST_METHOD(TestRegisterParameterHandler_ErrorInvalidCommand)
      {
         // set up
         ProgramOptions options;
         options.RegisterOutputHandler(&ProgramOptions::OutputConsole);

         bool paramHandlerCalled = false;
         options.RegisterParameterHandler(
            [&paramHandlerCalled](const CString& param)
            {
               paramHandlerCalled = param == "clone";
               return paramHandlerCalled;
            });

         // run
         options.Parse(_T("dummy.exe cloneaaaargh"));

         // check
         Assert::IsFalse(paramHandlerCalled, L"parameter handler must not have been called");
      }
   };

} // namespace UnitTest
