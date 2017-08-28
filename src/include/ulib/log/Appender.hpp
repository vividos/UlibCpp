//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file Appender.hpp appender class
//
#pragma once

// includes
#include <ulib/log/Log.hpp>
#include <ulib/log/Layout.hpp>

namespace Log
{
   /// \brief appender base class
   /// \details appender do the output using their set layout class
   class Appender
   {
   public:
      /// ctor
      Appender()
      {
         // nothing to do
      }

      /// dtor
      virtual ~Appender()
      {
         // nothing to cleanup
      }

      /// returns layout object being used
      LayoutPtr Layout() { return m_layout; }

      /// sets new layout object
      void Layout(LayoutPtr layout) { m_layout = layout; }

      /// appends logging event to output
      virtual void DoAppend(const LoggingEventPtr loggingEvent) = 0;

   private:
      /// layout object
      LayoutPtr m_layout;
   };

} // namespace Log
