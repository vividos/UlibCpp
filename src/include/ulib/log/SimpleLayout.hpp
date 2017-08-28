//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file SimpleLayout.hpp simple layout class
//
#pragma once

// includes
#include <ulib/log/Layout.hpp>

namespace Log
{
   /// \brief simple layout
   /// \details prints out a simple text containing the log level and the message
   class SimpleLayout : public Layout
   {
   public:
      /// dtor
      virtual ~SimpleLayout()
      {
         // nothing to cleanup
      }

      /// formats the simple log message
      virtual void Format(CString& outputText, const LoggingEventPtr loggingEvent)
      {
         ATLASSERT(loggingEvent.get() != nullptr);

         switch (loggingEvent->Level())
         {
         case debug: outputText = _T("DEBUG"); break;
         case info:  outputText = _T("INFO "); break;
         case warn:  outputText = _T("WARN "); break;
         case error: outputText = _T("ERROR"); break;
         case fatal: outputText = _T("FATAL"); break;
         default:
            outputText = _T("unknown"); break;
         }

         outputText += _T(" - ");
         outputText += loggingEvent->Message();
      }
   };

} // namespace Log
