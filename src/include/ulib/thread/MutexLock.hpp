//
// ulib - a collection of useful classes
// Copyright (C) 2013-2014,2017,2019,2020 Michael Fink
//
/// \file MutexLock.hpp generic mutex lock classes
//
#pragma once

/// lock class for synchronisation objects
template <typename T>
class MutexLock
{
public:
   /// ctor; takes a lockable object
   explicit MutexLock(T& mutex)
      :m_mutex(mutex)
   {
      mutex.Lock();
   }

   /// dtor; unlocks object
   ~MutexLock()
   {
      m_mutex.Unlock();
   }

   /// move ctor; moves an already locked mutex to this object
   MutexLock(MutexLock&& lock)
      :m_mutex(std::move(lock.m_mutex))
   {
   }

   /// move assignment operator
   MutexLock& operator=(MutexLock&& lock)
   {
      m_mutex = std::move(lock.m_mutex);
   }

private:
   MutexLock(const MutexLock&) = delete;              ///< removed copy ctor
   MutexLock& operator=(const MutexLock&) = delete;   ///< removed assign op

private:
   /// locked object
   T& m_mutex;
};

/// mutex lock class that initially doesn't lock mutex, and gives a method to try locking the mutex
template <typename T>
class MutexTryLock
{
public:
   /// ctor; takes a lockable object, but doesn't lock it yet
   explicit MutexTryLock(T& mutex)
      :m_mutex(mutex)
   {
   }

   /// dtor; unlocks object
   ~MutexTryLock()
   {
      m_mutex.Unlock();
   }

   /// move ctor; moves an already locked mutex to this object
   MutexTryLock(MutexTryLock&& lock)
      :m_mutex(std::move(lock.m_mutex))
   {
   }

   /// move assignment operator
   MutexTryLock& operator=(MutexTryLock&& lock)
   {
      m_mutex = std::move(lock.m_mutex);
   }

   /// tries locking mutex until timeout (in milliseconds)
   bool Try(DWORD timeoutInMilliseconds)
   {
      return m_mutex.TryLock(timeoutInMilliseconds);
   }

private:
   MutexTryLock(const MutexTryLock&) = delete;              ///< removed copy ctor
   MutexTryLock& operator=(const MutexTryLock&) = delete;   ///< removed assign op

private:
   /// locked object
   T& m_mutex;
};

/// class for temporarily unlocking synchronisation objects
template <typename T>
class MutexUnLocker
{
public:
   /// ctor; takes a lockable object and unlocks it
   explicit MutexUnLocker(T& mutex)
      :m_mutex(mutex)
   {
      mutex.Unlock();
   }

   /// dtor; locks object
   ~MutexUnLocker()
   {
      m_mutex.Lock();
   }

   /// move ctor; moves an already locked mutex to this object
   MutexUnLocker(MutexUnLocker&& lock)
      :m_mutex(std::move(lock.m_mutex))
   {
   }

   /// move assignment operator
   MutexUnLocker& operator=(MutexUnLocker&& lock)
   {
      m_mutex = std::move(lock.m_mutex);
   }

private:
   MutexUnLocker(const MutexUnLocker&) = delete;            ///< removed copy ctor
   MutexUnLocker& operator=(const MutexUnLocker&) = delete; ///< removed assign op

private:
   /// unlocked object
   T& m_mutex;
};
