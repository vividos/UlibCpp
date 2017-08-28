//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file PatternLayout.hpp pattern layout class
//
#pragma once

// includes
#include <ulib/log/Layout.hpp>

namespace Log
{
   /// \brief pattern layout
   /// \details Uses a format pattern string to format a log line from logging
   /// events. A pattern string consists of text and pattern specifiers that start
   /// with the % char and end with a pattern character, similar to the printf
   /// notation. The following characters have meaning:
   /// - c: outputs logger name
   /// - d: date in ISO 8601 format
   /// - F: source filename where log message occured
   /// - l: not supported
   /// - L: source file line where log message occured
   /// - m: log message; not supported
   /// - n: platform-specific newline character
   /// - p: log level (priority)
   /// - r: not supported
   /// - t: thread id
   /// - %: percent sign
   ///
   /// The following format modifiers are allowed:
   /// - The - adds left justification to the string
   /// - Next comes the minimum field width (excess space is padded)
   /// - Optional . (dot)
   /// - Next comes the maximum field width; if string is larger, the last n characters are shown
   class PatternLayout : public Layout
   {
   public:
      /// ctor
      explicit PatternLayout(const CString& pattern)
         :m_pattern(pattern)
      {
         // nothing to do
      }

      /// dtor
      virtual ~PatternLayout()
      {
         // nothing to cleanup
      }

      /// formats output using pattern
      virtual void Format(CString& outputText, const LoggingEventPtr loggingEvent);

   private:
      /// pattern to use
      CString m_pattern;
   };

} // namespace Log
