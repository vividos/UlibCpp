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
#include "Exception.hpp"
#include "FileFinder.hpp"
#include "IoCContainer.hpp"
#include "Observer.hpp"
#include "Path.hpp"
#include "ProgramOptions.hpp"
#include "SystemException.hpp"
#include "Timer.hpp"
#include "UTF8.hpp"

#include "thread/Event.hpp"
#include "thread/LightweightMutex.hpp"
#include "thread/Mutex.hpp"
#include "thread/MutexLock.hpp"
#include "thread/RecursiveMutex.hpp"
#include "thread/Thread.hpp"

#include "win32/ErrorMessage.hpp"
#include "win32/IniFile.hpp"
#include "win32/Process.hpp"

#include "unittest/AutoCleanupFile.hpp"
#include "unittest/AutoCleanupFolder.hpp"
