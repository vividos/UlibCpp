//
// ulib - a collection of useful classes
// Copyright (C) 2020 Michael Fink
//
/// \file TestLogger.cpp unit test for Logger classes
//
#include "stdafx.h"
#include <ulib/log/Log.hpp>
#include <ulib/log/Logger.hpp>
#include <ulib/log/Appender.hpp>
#include <ulib/log/OutputDebugStringAppender.hpp>
#include <ulib/log/ConsoleAppender.hpp>
#include <ulib/log/Layout.hpp>
#include <ulib/log/SimpleLayout.hpp>
#include <ulib/log/PatternLayout.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// \brief test appender
   /// records all log messages in the vector accessible by Messages()
   class TestAppender : public Log::Appender
   {
   public:
      /// dtor
      virtual ~TestAppender() {}

      //// counts logging output and stores message
      virtual void DoAppend(const Log::LoggingEventPtr loggingEvent) override
      {
         CString output;
         Layout()->AppendHeader(output);
         Layout()->Format(output, loggingEvent);
         Layout()->AppendFooter(output);

         m_messageList.push_back(output);
      }

      /// returns all collected messages
      const std::vector<CString>& Messages() const { return m_messageList; }

   private:
      /// all collected messages
      std::vector<CString> m_messageList;
   };

   /// \brief sets up test logging
   /// Sets up root logger to have a certain logger category. At destruction of this object
   /// all appender of the root logger and the specified logger are removed.
   class TestLoggerSetup
   {
   public:
      /// ctor
      TestLoggerSetup(const CString& loggerCategory)
      {
         Log::Logger::GetRootLogger()->RemoveAllAppender();

         // add logger to given category
         m_logger = Log::Logger::GetLogger(loggerCategory);
      }

      /// dtor; cleans up root and custom logger
      ~TestLoggerSetup()
      {
         Log::Logger::GetRootLogger()->RemoveAllAppender();
         m_logger->RemoveAllAppender();
      }

      /// returns custom logger
      Log::LoggerPtr Logger() { return m_logger; }

   private:
      // logger
      Log::LoggerPtr m_logger;
   };

   /// Tests for Logger
   TEST_CLASS(TestLogger)
   {
      /// tests logging
      TEST_METHOD(TestLogging)
      {
         Log::AppenderPtr appender1{ new Log::OutputDebugStringAppender };
         appender1->Layout(Log::LayoutPtr{ new Log::SimpleLayout });

         Log::AppenderPtr appender2(new Log::ConsoleAppender);
         appender2->Layout(Log::LayoutPtr(new Log::PatternLayout(
            _T("Log c=[%20c] d=[%.20d] F=[%20.20F] l=[%l] L=[%-20L] m=[%-.20m] n=[%-20.20n] p=[%p] r=[%r] t=[%t] %%=[%%] Log"))));

         Log::Logger::GetRootLogger()->AddAppender(appender1);

         LOG_DEBUG(_T("test-msg1"), _T("cat"));
         LOG_FATAL(_T("test-msg1"), _T("cat"));
         LOG_INFO(_T("test-msg1"), _T("cat.abc"));

         Log::Logger::GetLogger(_T("cat"))->AddAppender(appender2);

         LOG_WARN(_T("test-msg1"), _T("cat.abc.123"));
         LOG_ERROR(_T("test-msg1"), _T("cat.abc.456"));

         Log::Logger::GetRootLogger()->RemoveAllAppender();

         LOG_FATAL(_T("test-msg1"), _T("cat.def.123"));

         Log::Logger::GetLogger(_T("cat"))->RemoveAllAppender();
      }

      /// tests PatternLayout class
      TEST_METHOD(TestPatternLayout)
      {
         LPCTSTR c_category = _T("cat.log");
         TestLoggerSetup setup{ c_category };

         std::shared_ptr<TestAppender> appender(new TestAppender);
         setup.Logger()->AddAppender(appender);

         LPCTSTR testMessage = _T("abc123");

         struct TestData
         {
            LPCTSTR pattern;
            LPCTSTR expectedResult;
         } testData[] =
         {
            // tests for format specifiers modifying min/max length
            { _T("%m"),       _T("abc123") },      // without modification
            { _T("%2m"),      _T("abc123") },      // min chars < size
            { _T("%0m"),      _T("abc123") },      // min width zero: should work
            { _T("%10m"),     _T("    abc123") },  // left padding
            { _T("%-10m"),    _T("abc123    ") },  // right padding
            { _T("%.5m"),     _T("bc123") },       // max chars, truncate to 5
            { _T("%-.5m"),    _T("bc123") },       // max chars again, left justify flag isn't used here
            { _T("%5.5m"),    _T("bc123") },       // truncate to 5
            { _T("%7.5m"),    _T("bc123") },       // left pad 1 char, then right truncate to 5
            { _T("%-7.5m"),   _T("c123 ") },       // right pad 1 char, then right truncate to 5
            { _T("%.m"),      _T("abc123") },      // no change
            { _T("%.0m"),     _T("") },            // max size zero: should work, makes no sense though
         };

         for (size_t i = 0; i < sizeof_array(testData); i++)
         {
            appender->Layout(Log::LayoutPtr(new Log::PatternLayout(testData[i].pattern)));

            LOG_INFO(testMessage, c_category);

            // each logging must generate a message
            Assert::AreEqual(i + 1, appender->Messages().size(), L"number of messages must match");

            // check generated message
            CString result = appender->Messages().back();
            Assert::AreEqual(testData[i].expectedResult, result, L"result must match");
         }
      }
   };

} // namespace UnitTest
