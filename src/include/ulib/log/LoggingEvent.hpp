//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file LoggingEvent.hpp logging event class
//
#pragma once

// includes
#include <ulib/log/Log.hpp>
#include <ulib/DateTime.hpp>
#include <ulib/thread/Thread.hpp>
#include <vector>

namespace Log
{
   /// logging event
   class LoggingEvent
   {
   public:
      /// ctor
      LoggingEvent(Log::Level level, const CString& loggerName, const CString& message,
         const CString& sourceFilename, UINT sourceLine)
         :m_level(level),
         m_loggerName(loggerName),
         m_message(message),
         m_sourceFilename(sourceFilename),
         m_sourceLine(sourceLine),
         m_threadId(Thread::CurrentId()),
         m_timestamp(DateTime::Now())
      {
      }

      /// log level
      Log::Level Level() const { return m_level; }

      /// logger name
      CString LoggerName() const { return m_loggerName; }

      /// message
      CString Message() const { return m_message; }

      /// source filename
      CString SourceFilename() const { return m_sourceFilename; }

      /// sourcefile line
      UINT SourceLine() const { return m_sourceLine; }

      /// thread id
      DWORD ThreadId() const { return m_threadId; }

      /// timestamp of the event
      const DateTime& Timestamp() const { return m_timestamp; }

      /// parameter count
      size_t ParamCount() const { return m_paramsList.size(); }

      /// returns parameter
      CString Param(unsigned int paramIndex) const;

   private:
      /// level
      Log::Level m_level;

      /// logger name
      CString m_loggerName;

      /// log message
      CString m_message;

      /// source filename
      CString m_sourceFilename;

      /// sourcefile line
      UINT m_sourceLine;

      /// thread id
      DWORD m_threadId;

      /// event timestamp
      DateTime m_timestamp;

      /// message parameter
      std::vector<CString> m_paramsList;
   };

} // namespace Log
