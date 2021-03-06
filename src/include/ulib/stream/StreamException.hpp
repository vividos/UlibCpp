//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2014,2017 Michael Fink
//
/// \file StreamException.hpp exception used by stream classes
//
#pragma once

#include <ulib/Exception.hpp>

namespace Stream
{
   /// exception that is thrown when an error in a stream method occurs
   class StreamException : public Exception
   {
   public:
      /// ctor; takes source filename and line number
      StreamException(LPCSTR sourceFile, UINT sourceLine)
         :Exception(sourceFile, sourceLine)
      {
      }

      /// ctor; takes message and source filename and line number
      StreamException(const CString& message, LPCSTR sourceFile, UINT sourceLine)
         :Exception(message, sourceFile, sourceLine)
      {
      }
   };

} // namespace Stream
