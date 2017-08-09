//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file RecursiveMutex.hpp recursive mutex class
//
#pragma once

#include <memory>
#include "MutexLock.hpp"
#include "SystemException.hpp"

/// a recursive lockable synchronisation object
class RecursiveMutex
{
public:
   /// lock type
   typedef MutexLock<RecursiveMutex> LockType;

   /// try-lock type
   typedef MutexTryLock<RecursiveMutex> TryLockType;

   /// ctor
   RecursiveMutex()
   {
      HANDLE mutexHandle = ::CreateMutex(nullptr, FALSE, nullptr);
      if (mutexHandle == NULL)
         throw SystemException(_T("failed to create mutex"), GetLastError(), __FILE__, __LINE__);

      m_mutex.reset(mutexHandle, ::CloseHandle);
   }

private:
   /// locks object
   void Lock()
   {
      DWORD ret = ::WaitForSingleObject(m_mutex.get(), INFINITE);
      if (ret == WAIT_OBJECT_0)
         return;

      if (ret == WAIT_ABANDONED)
         throw SystemException(_T("try locking abandoned mutex"), ret, __FILE__, __LINE__);

      // unknown failure
      throw SystemException(_T("failed to lock recursive mutex"), GetLastError(), __FILE__, __LINE__);
   }

   /// tries locking object, with timeout (in milliseconds)
   bool TryLock(DWORD timeoutInMilliseconds)
   {
      DWORD ret = ::WaitForSingleObject(m_mutex.get(), timeoutInMilliseconds);
      if (ret == WAIT_OBJECT_0)
         return true;

      if (ret == WAIT_TIMEOUT)
         return false;

      if (ret == WAIT_ABANDONED)
         throw SystemException(_T("try locking abandoned mutex"), ret, __FILE__, __LINE__);

      // unknown failure
      throw SystemException(_T("failed to lock recursive mutex"), GetLastError(), __FILE__, __LINE__);
   }

   /// locks object
   void Unlock()
   {
      BOOL ret = ::ReleaseMutex(m_mutex.get());
      if (ret == FALSE)
         ATLTRACE(_T("RecursiveMutex::Unlock() error: didn't own mutex!\n"));
   }

   // so that Lock class can call Lock(), Try() and Unlock()
   friend MutexLock<RecursiveMutex>;
   friend MutexTryLock<RecursiveMutex>;
   friend MutexUnLocker<RecursiveMutex>;

private:
   /// mutex handle
   std::shared_ptr<void> m_mutex;
};
