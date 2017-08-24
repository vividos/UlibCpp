//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file ConsoleAppender.hpp console appender class
//
#pragma once

// includes
#include <ulib/log/Appender.hpp>

namespace Log
{
   /// \brief console appender
   /// \details outputs formatted text to the stdout console stream
   class ConsoleAppender : public Appender
   {
   public:
      /// dtor
      virtual ~ConsoleAppender() {}

      /// outputs logging event using _tprintf
      virtual void DoAppend(const LoggingEventPtr loggingEvent)
      {
         ATLASSERT(Layout().get() != nullptr);

         CString outputText;
         Layout()->Format(outputText, loggingEvent);

         _tprintf(_T("%s\n"), outputText.GetString());
      }
   };

} // namespace Log
