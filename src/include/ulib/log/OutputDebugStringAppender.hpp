//
// ulib - a collection of useful classes
// Copyright (C) 2006-2015,2017 Michael Fink
//
/// \file OutputDebugStringAppender.hpp appender that is using OutputDebugString()
//
#pragma once

// includes
#include <ulib/log/Appender.hpp>

namespace Log
{
   /// \brief appender that uses OutputDebugString on Win32
   /// \details outputs formatted text to the debug console
   class OutputDebugStringAppender : public Appender
   {
   public:
      /// dtor
      virtual ~OutputDebugStringAppender()
      {
         // nothing to cleanup
      }

      /// outputs logging event using OutputDebugString
      virtual void DoAppend(const LoggingEventPtr loggingEvent)
      {
         ATLASSERT(Layout().get() != nullptr);

         CString outputText;
         Layout()->Format(outputText, loggingEvent);

#ifdef _MSC_VER
         OutputDebugString(outputText);
         OutputDebugString(_T("\n"));
#endif
      }
   };

} // namespace Log
