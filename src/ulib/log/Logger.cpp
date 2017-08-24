//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file Logger.cpp Logger implementation
//

// needed includes
#include "stdafx.h"
#include <ulib/log/Logger.hpp>
#include <ulib/log/Appender.hpp>
#include <ulib/log/LoggingEvent.hpp>

#define BOOST_THREAD_NO_LIB
#include <boost/thread/once.hpp>

using Log::Logger;
using Log::LoggerPtr;

CString Logger::Name()
{
   // check if parent is the root logger
   if (Parent() == NULL)
      return _T("");

   if (Parent()->Parent() == NULL)
      return m_name; // just one above the parent
   else
      return Parent()->Name() + _T(".") + m_name;
}

/// once flag for root logger initialisation
boost::once_flag g_rootLoggerOnceFlag = BOOST_ONCE_INIT;

/// root logger
static Log::LoggerPtr s_rootLogger;

void Logger::InitRootLogger()
{
   s_rootLogger = Log::LoggerPtr(new Log::Logger(_T(""), Log::LoggerPtr()));
}

LoggerPtr Logger::GetRootLogger()
{
   boost::call_once(g_rootLoggerOnceFlag, &Logger::InitRootLogger);
   return s_rootLogger;
}

LoggerPtr Logger::GetLogger(const CString& name)
{
   LoggerPtr logger = GetRootLogger();
   ATLASSERT(logger != NULL); // should not happen!

   // recursively search logger map for logger
   int pos = 0, maxPos = name.GetLength();
   while (pos < maxPos)
   {
      CString loggerName;

      // check if there's a dot
      int posDot = name.Find(_T('.'), pos);
      if (posDot != -1)
      {
         loggerName = name.Mid(pos, posDot - pos);
         pos = posDot + 1;
      }
      else
      {
         // no further dots
         loggerName = name.Mid(pos);
         pos = maxPos;
      }

      loggerName.MakeLower();

      // search logger name
      T_mapLoggerMap::const_iterator iter = logger->m_mapChildLogger.find(loggerName);
      if (iter == logger->m_mapChildLogger.end())
      {
         // no logger; create it
         LoggerPtr spNewLogger(new Logger(loggerName, logger));
         logger->m_mapChildLogger.insert(std::make_pair(loggerName, spNewLogger));
         logger = spNewLogger;
      }
      else
      {
         // found logger; use as next (or last)
         logger = iter->second;
      }
   }

   return logger;
}

void Logger::Log(Log::Level level, const CString& message, LPCSTR filename, UINT lineNumber)
{
   ATLASSERT(level != none);

   // find out logger level
   Log::Level loggerLevel = GetLoggerLevel();

   if (level >= loggerLevel)
   {
      // send message to all appenders
      CString sourceFilename(filename);

      LoggingEventPtr event(
         new LoggingEvent(level, Name(), message, sourceFilename, lineNumber));

      LogEvent(event);
   }
}

void Logger::LogEvent(Log::LoggingEventPtr event)
{
   CallAppenders(event);

   // when logger is additive (and not a root logger), send to parent
   if (m_additivity && m_parentLogger.get() != nullptr)
      Parent()->LogEvent(event);
}

Logger::Logger(const CString& name, LoggerPtr parentLogger)
   :m_level(none),
   m_additivity(true),
   m_parentLogger(parentLogger),
   m_name(name)
{
   // construct a root logger?
   if (parentLogger.get() == nullptr)
   {
      m_level = debug;
      m_additivity = false;
   }
}

Log::Level Logger::GetLoggerLevel()
{
   Log::Level loggerLevel = m_level;
   if (loggerLevel == none)
   {
      LoggerPtr parentLogger = m_parentLogger;
      do
      {
         ATLASSERT(parentLogger.get() != NULL);
         loggerLevel = parentLogger->Level();
         parentLogger = parentLogger->Parent();
      } while (loggerLevel == none);
   }

   return loggerLevel;
}

void Logger::CallAppenders(const Log::LoggingEventPtr loggingEvent)
{
   T_setAppender::const_iterator iter = m_setAppender.begin(), stop = m_setAppender.end();
   for (; iter != stop; ++iter)
      (*iter)->DoAppend(loggingEvent);
}
