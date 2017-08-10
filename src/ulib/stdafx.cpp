//
// ulib - a collection of useful classes
// Copyright (C) 2017 Michael Fink
//
/// \file ulib/stdafx.cpp Precompiled header support
//

#include "stdafx.h"

// include all public headers here, in order to check if they compile
#include "CommandLineParser.hpp"
#include "CrashReporter.hpp"
#include "DynamicLibrary.hpp"
#include "Event.hpp"
#include "Exception.hpp"
#include "FileFinder.hpp"
#include "IoCContainer.hpp"
#include "LightweightMutex.hpp"
#include "Mutex.hpp"
#include "MutexLock.hpp"
#include "Observer.hpp"
#include "Path.hpp"
#include "ProgramOptions.hpp"
#include "RecursiveMutex.hpp"
#include "SystemException.hpp"
#include "Thread.hpp"
#include "Timer.hpp"
#include "UTF8.hpp"

#include "win32/ErrorMessage.hpp"
#include "win32/IniFile.hpp"
#include "win32/Process.hpp"

#include "unittest/AutoCleanupFile.hpp"
#include "unittest/AutoCleanupFolder.hpp"
