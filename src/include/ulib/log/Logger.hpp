//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file Logger.hpp logger class
//
#pragma once

// includes
#include <ulib/log/Log.hpp>
#include <memory>
#include <map>
#include <set>

namespace Log
{
   /// logging event smart pointer
   typedef std::shared_ptr<class LoggingEvent> LoggingEventPtr;

   /// logger
   class Logger
   {
   public:
      // properties

      /// returns logger level
      Level Level() const { return m_level; }

      /// returns additivity flag
      bool Additivity() const { return m_additivity; }

      /// returns parent logger
      LoggerPtr Parent() { return m_parentLogger; }

      /// returns full logger name
      CString Name();

      /// sets logger level
      void Level(Log::Level level) { m_level = level; }

      /// sets additivity
      void Additivity(bool additivity) { m_additivity = additivity; }


      // methods

      /// returns root logger
      static LoggerPtr GetRootLogger();

      /// returns logger with given name
      static LoggerPtr GetLogger(const CString& name);

      /// adds appender to this logger
      void AddAppender(AppenderPtr appender)
      {
         m_setAppender.insert(appender);
      }

      /// removes all appender from this logger
      void RemoveAllAppender()
      {
         m_setAppender.clear();
      }

      /// removes given appender
      void RemoveAppender(AppenderPtr appender)
      {
         T_setAppender::iterator iter = m_setAppender.find(appender);
         if (iter != m_setAppender.end())
            m_setAppender.erase(iter);
      }

      /// logs a debug message
      void Debug(const CString& message, LPCSTR filename, UINT lineNumber)
      {
         Log(debug, message, filename, lineNumber);
      }

      /// logs an info message
      void Info(const CString& message, LPCSTR filename, UINT lineNumber)
      {
         Log(info, message, filename, lineNumber);
      }

      /// logs a warning message
      void Warn(const CString& message, LPCSTR filename, UINT lineNumber)
      {
         Log(warn, message, filename, lineNumber);
      }

      /// logs an error message
      void Error(const CString& message, LPCSTR filename, UINT lineNumber)
      {
         Log(error, message, filename, lineNumber);
      }

      /// logs a fatal error message
      void Fatal(const CString& message, LPCSTR filename, UINT lineNumber)
      {
         Log(fatal, message, filename, lineNumber);
      }

      /// logs a message with given level
      void Log(Log::Level enLevel, const CString& message, LPCSTR filename, UINT lineNumber);

      /// logs a logging event
      void LogEvent(LoggingEventPtr event);

   private:
      /// ctor; logger can only be created by itself
      Logger(const CString& name, LoggerPtr parentLogger);

      /// inits root logger
      static void InitRootLogger();

      /// determines logger level; asks parent if no level was set
      Log::Level GetLoggerLevel();

      /// call all appenders using given event
      void CallAppenders(const LoggingEventPtr event);

   private:
      /// logger level
      Log::Level m_level;

      /// additivity; when true, log events are sent to parent, too
      bool m_additivity;

      /// parent
      LoggerPtr m_parentLogger;

      /// logger name
      CString m_name;

      /// map type for child logger
      typedef std::map<CString, LoggerPtr> T_mapLoggerMap;
      /// logger name / child logger mapping
      T_mapLoggerMap m_mapChildLogger;

      /// set type containing appender
      typedef std::set<AppenderPtr> T_setAppender;
      /// set with all appenders for this logger
      T_setAppender m_setAppender;
   };

} // namespace Log

/// logs using DEBUG level
#define LOG_DEBUG(msg, cat) ::Log::Logger::GetLogger(cat)->Debug(msg, __FILE__, __LINE__);
/// logs using INFO level
#define LOG_INFO(msg, cat) ::Log::Logger::GetLogger(cat)->Info(msg, __FILE__, __LINE__);
/// logs using WARN level
#define LOG_WARN(msg, cat) ::Log::Logger::GetLogger(cat)->Warn(msg, __FILE__, __LINE__);
/// logs using ERROR level
#define LOG_ERROR(msg, cat) ::Log::Logger::GetLogger(cat)->Error(msg, __FILE__, __LINE__);
/// logs using FATAL level
#define LOG_FATAL(msg, cat) ::Log::Logger::GetLogger(cat)->Fatal(msg, __FILE__, __LINE__);
