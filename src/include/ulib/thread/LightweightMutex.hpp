//
// ulib - a collection of useful classes
// Copyright (C) 2008-2014,2017 Michael Fink
//
/// \file LightweightMutex.hpp Lightweight mutex class
//
#pragma once

#include "MutexLock.hpp"

/// \brief lightweight, non-recursive mutex
/// \details a mutex that is optimized for non-contention cases
/// \see http://preshing.com/20111124/always-use-a-lightweight-mutex/
class LightweightMutex
{
public:
   /// lock type
   typedef MutexLock<LightweightMutex> LockType;

   /// ctor
   LightweightMutex()
   {
#pragma warning(disable: 6320) // Exception-filter expression is the constant EXCEPTION_EXECUTE_HANDLER. This might mask exceptions that were not intended to be handled.
#pragma warning(disable: 6322) // Empty _except block.
      __try
      {
         InitializeCriticalSection(&m_criticalSection);
      }
      __except (EXCEPTION_EXECUTE_HANDLER)
      {
         // the function can raise STATUS_NO_MEMORY exception on Windows XP
      }
#pragma warning(default: 6320)
#pragma warning(default: 6322)
   }

   /// dtor
   ~LightweightMutex()
   {
      DeleteCriticalSection(&m_criticalSection);
   }

private:
   /// locks mutex
   void Lock()
   {
      EnterCriticalSection(&m_criticalSection);
   }

   /// unlocks mutex
   void Unlock()
   {
      LeaveCriticalSection(&m_criticalSection);
   }

   // friend so that MutexLock class can call Lock() and Unlock()
   friend MutexLock<LightweightMutex>;

private:
   /// critical section object
   CRITICAL_SECTION m_criticalSection;
};
