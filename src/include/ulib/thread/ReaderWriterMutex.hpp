//
// ulib - a collection of useful classes
// Copyright (C) 2009,2012,2017 Michael Fink
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
   /// dtor
   ~ReaderWriterMutex();

private:
   ReaderWriterMutex(const ReaderWriterMutex&) = delete;              ///< removed copy ctor
   ReaderWriterMutex& operator=(const ReaderWriterMutex&) = delete;   ///< removed assign op

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
