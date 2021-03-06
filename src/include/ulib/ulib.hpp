//
// ulib - a collection of useful classes
// Copyright (C) 2020 Michael Fink
//
/// \file ulib.hpp all ulib includes
//
#pragma once

#include <ulib/CommandLineParser.hpp>
#include <ulib/CrashReporter.hpp>
#include <ulib/DateTime.hpp>
#include <ulib/DynamicLibrary.hpp>
#include <ulib/Exception.hpp>
#include <ulib/FileFinder.hpp>
#include <ulib/HighResolutionTimer.hpp>
#include <ulib/IoCContainer.hpp>
#include <ulib/Observer.hpp>
#include <ulib/Path.hpp>
#include <ulib/ProgramOptions.hpp>
#include <ulib/Singleton.hpp>
#include <ulib/SystemException.hpp>
#include <ulib/Timer.hpp>
#include <ulib/TimeSpan.hpp>
#include <ulib/TimeZone.hpp>
#include <ulib/TraceOutputStopwatch.hpp>
#include <ulib/UTF8.hpp>

#include <ulib/log/AndroidLogcatAppender.hpp>
#include <ulib/log/Appender.hpp>
#include <ulib/log/ConsoleAppender.hpp>
#include <ulib/log/Layout.hpp>
#include <ulib/log/Log.hpp>
#include <ulib/log/Logger.hpp>
#include <ulib/log/LoggingEvent.hpp>
#include <ulib/log/OutputDebugStringAppender.hpp>
#include <ulib/log/PatternLayout.hpp>
#include <ulib/log/SimpleLayout.hpp>
#include <ulib/log/TextStreamAppender.hpp>

#include <ulib/stream/EndianAwareFilter.hpp>
#include <ulib/stream/FileStream.hpp>
#include <ulib/stream/IStream.hpp>
#include <ulib/stream/ITextStream.hpp>
#include <ulib/stream/MemoryReadStream.hpp>
#include <ulib/stream/MemoryStream.hpp>
#include <ulib/stream/NullStream.hpp>
#include <ulib/stream/StreamException.hpp>
#include <ulib/stream/TextFileStream.hpp>
#include <ulib/stream/TextStreamFilter.hpp>

#include <ulib/thread/Event.hpp>
#include <ulib/thread/LightweightMutex.hpp>
#include <ulib/thread/Mutex.hpp>
#include <ulib/thread/MutexLock.hpp>
#include <ulib/thread/RecursiveMutex.hpp>
#include <ulib/thread/Thread.hpp>

#include <ulib/win32/DocHostUI.hpp>
#include <ulib/win32/ErrorMessage.hpp>
#include <ulib/win32/IniFile.hpp>
#include <ulib/win32/Process.hpp>
#include <ulib/win32/Win7Taskbar.hpp>

#include <ulib/unittest/AutoCleanupFile.hpp>
#include <ulib/unittest/AutoCleanupFolder.hpp>
