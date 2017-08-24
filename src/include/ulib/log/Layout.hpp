//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file Layout.hpp layout class
//
#pragma once

// includes
#include <ulib/log/Log.hpp>
#include <ulib/log/LoggingEvent.hpp>

namespace Log
{
   /// \brief log layout base class
   /// \details the layout class formats the logging event for output
   class Layout
   {
   public:
      /// dtor
      virtual ~Layout() {}

      /// appends header to output; override when necessary
      virtual void AppendHeader(CString& outputText) { outputText; }

      /// appends footer to output; override when necessary
      virtual void AppendFooter(CString& outputText) { outputText; }

      /// formats logging event to string
      virtual void Format(CString& outputText, const LoggingEventPtr loggingEvent) = 0;
   };

} // namespace Log
