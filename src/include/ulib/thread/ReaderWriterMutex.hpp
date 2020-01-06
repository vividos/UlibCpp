//
// ulib - a collection of useful classes
// Copyright (C) 2009,2012,2017,2020 Michael Fink
//
/// \file ReaderWriterMutex.hpp reader/writer mutex
//
#pragma once

#include <memory>

/// \brief reader-writer mutex
class ReaderWriterMutex
{
public:
   /// ctor
   ReaderWriterMutex();

   /// copy ctor; not available
   ReaderWriterMutex(const ReaderWriterMutex& other) = delete;

   // move ctor
   ReaderWriterMutex(ReaderWriterMutex&& other) noexcept
      :m_spImpl(std::move(other.m_spImpl))
   {
   }

   /// dtor
   ~ReaderWriterMutex();

   /// copy assignment operator; not available
   ReaderWriterMutex& operator=(const ReaderWriterMutex& other) = delete;

   /// move assignment operator
   ReaderWriterMutex& operator=(ReaderWriterMutex&& other) noexcept
   {
      m_spImpl = std::move(other.m_spImpl);
      return *this;
   }

private:
   friend class ReaderLock;
   friend class WriterLock;

   class Impl;

   /// implementation pointer
   std::shared_ptr<Impl> m_spImpl;
};

/// \brief reader lock
/// \details multiple reader locks are possible
class ReaderLock
{
public:
   /// ctor; locks the mutex as reader
   explicit ReaderLock(ReaderWriterMutex& mutex);
   /// dtor; releases the mutex
   ~ReaderLock();

private:
   ReaderLock(const ReaderLock&) = delete;              ///< removed copy ctor
   ReaderLock& operator=(const ReaderLock&) = delete;   ///< removed assign op

   /// ref to mutex
   ReaderWriterMutex& m_mutex;
};

/// \brief writer lock
/// \details only one writer lock is possible
class WriterLock
{
public:
   /// ctor; locks the mutex as writer
   explicit WriterLock(ReaderWriterMutex& mutex);
   /// dtor; releases the mutex
   ~WriterLock();

private:
   WriterLock(const WriterLock&) = delete;              ///< removed copy ctor
   WriterLock& operator=(const WriterLock&) = delete;   ///< removed assign op

   /// ref to mutex
   ReaderWriterMutex& m_mutex;
};
