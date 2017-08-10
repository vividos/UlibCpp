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
#include "ErrorMessage.hpp"
#include "Event.hpp"
#include "Exception.hpp"
#include "FileFinder.hpp"
#include "LightweightMutex.hpp"
#include "Mutex.hpp"
#include "MutexLock.hpp"
#include "Observer.hpp"
#include "Path.hpp"
#include "Process.hpp"
#include "ProgramOptions.hpp"
#include "RecursiveMutex.hpp"
#include "SystemException.hpp"
#include "Thread.hpp"
#include "Timer.hpp"
#include "UTF8.hpp"

#include "unittest\AutoCleanupFile.hpp"
#include "unittest\AutoCleanupFolder.hpp"
