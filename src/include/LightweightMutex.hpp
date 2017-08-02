//
// ulib - a collection of useful classes
// Copyright (C) 2008-2014 Michael Fink
//
/// \file LightweightMutex.hpp Lightweight mutex class
//
#pragma once

#include "MutexLock.hpp"

/// \brief lightweight, non-recursive mutex
/// \details a mutex that is optimized for non-contention cases
class LightweightMutex
{
public:
   /// lock type
   typedef MutexLock<LightweightMutex> LockType;

   /// ctor
   LightweightMutex()
   {
      __try
      {
         InitializeCriticalSection(&m_cs);
      }
      __except (EXCEPTION_EXECUTE_HANDLER)
      {
         // the function can raise STATUS_NO_MEMORY exception on Windows XP
      }
   }

   /// dtor
   ~LightweightMutex()
   {
      DeleteCriticalSection(&m_cs);
   }

private:
   /// locks mutex
   void Lock()
   {
      EnterCriticalSection(&m_cs);
   }

   /// unlocks mutex
   void Unlock()
   {
      LeaveCriticalSection(&m_cs);
   }

   // friend so that MutexLock class can call Lock() and Unlock()
   friend MutexLock<LightweightMutex>;

private:
   /// critical section object
   CRITICAL_SECTION m_cs;
};
