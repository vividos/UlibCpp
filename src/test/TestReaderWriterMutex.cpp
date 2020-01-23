//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012,2017,2020 Michael Fink
//
/// \file TestReaderWriterMutex.cpp tests for ReaderWriterMutex class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/thread/ReaderWriterMutex.hpp>
#include <ulib/HighResolutionTimer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   const int DEFAULT_READERS = 1;
   const int DEFAULT_WRITERS = 1;

   const int DEFAULT_ITERATIONS = 2000000;
   const int DEFAULT_READER_WORK_WHILE_HOLDING_LOCK = 0;
   const int DEFAULT_WRITER_WORK_WHILE_HOLDING_LOCK = 0;
   const int DEFAULT_READER_WORK_INBETWEEEN_LOCK_ACQUISTIONS = 0;
   const int DEFAULT_WRITER_WORK_INBETWEEEN_LOCK_ACQUISTIONS = 0;

   struct ProgramArgs
   {
      DWORD numReaderThreads;
      DWORD numWriterThreads;
      int numReaderIterations;
      int numWriterIterations;
      int readerWorkWhileHoldingLock;
      int writerWorkWhileHoldingLock;
      int readerWorkInBetweenLockAcquisitions;
      int writerWorkInBetweenLockAcquisitions;

      ProgramArgs()
         : numReaderThreads(DEFAULT_READERS),
         numWriterThreads(DEFAULT_WRITERS),
         numReaderIterations(DEFAULT_ITERATIONS),
         numWriterIterations(DEFAULT_ITERATIONS),
         readerWorkWhileHoldingLock(DEFAULT_READER_WORK_WHILE_HOLDING_LOCK),
         writerWorkWhileHoldingLock(DEFAULT_WRITER_WORK_WHILE_HOLDING_LOCK),
         readerWorkInBetweenLockAcquisitions(DEFAULT_READER_WORK_INBETWEEEN_LOCK_ACQUISTIONS),
         writerWorkInBetweenLockAcquisitions(DEFAULT_WRITER_WORK_INBETWEEEN_LOCK_ACQUISTIONS)
      {
      }
   };

   struct ThreadTimes
   {
      enum
      {
         Reader = 0,
         Writer = 1
      };

      int type = Reader;
      int updatesObserved = 0;
      HighResolutionTimer timer;
   };

   struct ThreadArgs
   {
      int numReaderIterations;
      int numWriterIterations;
      int readerWorkWhileHoldingLock;
      int writerWorkWhileHoldingLock;
      int readerWorkInBetweenLockAcquisitions;
      int writerWorkInBetweenLockAcquisitions;
      int thdIndex;
      void* valueHolder;
      HANDLE hThreadWaitingEvent;
      HANDLE hThreadWakeupEvent;
      ThreadTimes* threadTimes;
   };


   class ValueHolder
   {
   public:
      ValueHolder(ReaderWriterMutex& rwLock)
         : rwLock_(rwLock),
         value_(0),
         workAccum_(0)
      {
      }

      LONG getValue()
      {
         LONG result;

         {
            ReaderLock lock(rwLock_);

            result = value_;
         }

         return result;
      }

      LONG getValue(int fakeWork)
      {
         LONG result;

         {
            ReaderLock lock(rwLock_);

            for (int i = 0; i < fakeWork; ++i)
            {
               workAccum_ += i;
            }

            result = value_;
         }

         return result;
      }

      void setValue(LONG newValue)
      {
         WriterLock lock(rwLock_);

         value_ = newValue;
      }

      void updateValue(int discardValue, int fakeWork)
      {
         WriterLock lock(rwLock_);

         for (int i = 0; i < fakeWork; ++i)
         {
            workAccum_ += discardValue;
         }

         ++value_;
      }

   private:
      ReaderWriterMutex& rwLock_;
      LONG value_;

      // Dummy used to stall when doing fake work.
      int workAccum_;
   };

   unsigned int __stdcall ReaderThreadFunc(void* args)
   {
      ThreadArgs* threadArgs = static_cast<ThreadArgs*>(args);
      ThreadTimes* threadTimes = threadArgs->threadTimes + threadArgs->thdIndex;
      ValueHolder* valueHolder = static_cast<ValueHolder*>(threadArgs->valueHolder);
      int numIterations = threadArgs->numReaderIterations;

      threadTimes->type = ThreadTimes::Reader;

      SignalObjectAndWait(threadArgs->hThreadWaitingEvent, threadArgs->hThreadWakeupEvent, INFINITE, FALSE);

      threadTimes->timer.Start();

      LONG v = 0;

      if ((threadArgs->readerWorkInBetweenLockAcquisitions == 0) || (threadArgs->readerWorkWhileHoldingLock == 0))
      {
         for (int i = 0; i < numIterations; ++i)
         {
            v = valueHolder->getValue();
         }
      }
      else
      {
         int workWhileHoldingLock = threadArgs->readerWorkWhileHoldingLock;
         int workBetweenAcquisitions = threadArgs->readerWorkInBetweenLockAcquisitions;
         int updatesObserved = 0;
         int oldValue = 0;
         for (int i = 0; i < numIterations; ++i)
         {
            v = valueHolder->getValue(workWhileHoldingLock);

            if (v != oldValue)
            {
               ++updatesObserved;
               oldValue = v;
            }

            for (int w = 0; w < workBetweenAcquisitions; ++w)
            {
               v = v + (w % 10);
            }
         }

         threadTimes->updatesObserved = updatesObserved;
      }

      threadTimes->timer.Stop();

      return v;
   }

   unsigned int __stdcall WriterThreadFunc(void* args)
   {
      ThreadArgs* threadArgs = static_cast<ThreadArgs*>(args);
      ThreadTimes* threadTimes = threadArgs->threadTimes + threadArgs->thdIndex;
      ValueHolder* valueHolder = static_cast<ValueHolder*>(threadArgs->valueHolder);
      int numIterations = threadArgs->numWriterIterations;

      threadTimes->type = ThreadTimes::Writer;

      SignalObjectAndWait(threadArgs->hThreadWaitingEvent, threadArgs->hThreadWakeupEvent, INFINITE, FALSE);

      threadTimes->timer.Start();

      if ((threadArgs->writerWorkInBetweenLockAcquisitions == 0) && (threadArgs->writerWorkWhileHoldingLock == 0))
      {
         for (int i = 0; i < numIterations; ++i)
         {
            valueHolder->setValue(i);
         }
      }
      else
      {
         int workWhileHoldingLock = threadArgs->writerWorkWhileHoldingLock;
         int workBetweenAcquisitions = threadArgs->writerWorkInBetweenLockAcquisitions;
         int newV = 0;

         for (int i = 0; i < numIterations; ++i)
         {
            valueHolder->updateValue(newV, workWhileHoldingLock);

            for (int w = 0; w < workBetweenAcquisitions; ++w)
            {
               newV = newV + (w % 10);
            }
         }
      }

      threadTimes->timer.Stop();

      return numIterations;
   }

   /// tests reader/writer mutex
   TEST_CLASS(TestReaderWriterMutex)
   {
      /// tests reader/writer
      TEST_METHOD(TestReaderWriter)
      {
         // Must run on platform that supports QueryPerformanceFrequency.
         LARGE_INTEGER perfFreq;
         Assert::IsTrue(TRUE == QueryPerformanceFrequency(&perfFreq));

         unsigned(__stdcall * readerStartAddress) (void*);
         unsigned(__stdcall * writerStartAddress) (void*);

         ProgramArgs programArgs;

         programArgs.numReaderThreads = 1;
         programArgs.numWriterThreads = 1;
         DWORD totalThreads = programArgs.numReaderThreads + programArgs.numWriterThreads;

         ThreadArgs threadArgs;

         threadArgs.numReaderIterations = programArgs.numReaderIterations;
         threadArgs.numWriterIterations = programArgs.numWriterIterations;
         threadArgs.readerWorkWhileHoldingLock = programArgs.readerWorkWhileHoldingLock;
         threadArgs.writerWorkWhileHoldingLock = programArgs.writerWorkWhileHoldingLock;
         threadArgs.readerWorkInBetweenLockAcquisitions = programArgs.readerWorkInBetweenLockAcquisitions;
         threadArgs.writerWorkInBetweenLockAcquisitions = programArgs.writerWorkInBetweenLockAcquisitions;
         threadArgs.hThreadWaitingEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
         threadArgs.hThreadWakeupEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
         threadArgs.threadTimes = new ThreadTimes[totalThreads];

         ReaderWriterMutex rwLock;
         ValueHolder valHolder(rwLock);

         readerStartAddress = ReaderThreadFunc;
         writerStartAddress = WriterThreadFunc;
         threadArgs.valueHolder = &valHolder;

         HANDLE* threads = new HANDLE[totalThreads];

         HANDLE* nextThread = threads;

         int threadTimesIndex = 0;

         // Create the readers
         for (DWORD i = 0; i < programArgs.numReaderThreads; ++i, ++nextThread)
         {
            threadArgs.thdIndex = threadTimesIndex++;

            *nextThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, readerStartAddress, &threadArgs, FALSE, NULL));

            WaitForSingleObject(threadArgs.hThreadWaitingEvent, INFINITE);
         }

         // Create the writers
         for (DWORD i = 0; i < programArgs.numWriterThreads; ++i, ++nextThread)
         {
            threadArgs.thdIndex = threadTimesIndex++;

            *nextThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, writerStartAddress, &threadArgs, TRUE, NULL));

            WaitForSingleObject(threadArgs.hThreadWaitingEvent, INFINITE);
         }

         HighResolutionTimer globalTimer;

         globalTimer.Start();

         SetEvent(threadArgs.hThreadWakeupEvent);

         WaitForMultipleObjects(totalThreads, threads, TRUE, INFINITE);

         globalTimer.Stop();

         for (nextThread = threads; nextThread < (threads + totalThreads); ++nextThread)
         {
            CloseHandle(*nextThread);
         }

         delete[] threads;

         CloseHandle(threadArgs.hThreadWaitingEvent);
         CloseHandle(threadArgs.hThreadWakeupEvent);

         double timeInSecs;

         ATLTRACE(_T("Thread Timings:"));


         // Calculate timings.
         for (DWORD threadIndex = 0; threadIndex < totalThreads; ++threadIndex)
         {
            ThreadTimes* threadTimes = threadArgs.threadTimes + threadIndex;
            timeInSecs = threadTimes->timer.Elapsed();

            CString textUpdatesObserved;
            if (threadTimes->type == ThreadTimes::Reader)
               textUpdatesObserved.Format(_T(", updates observed: %i"), threadTimes->updatesObserved);

            CString text;
            text.Format(_T("\t%s %u duration %u.%03u secs.%s"),
               (threadTimes->type == ThreadTimes::Reader) ? _T("Reader") : _T("Writer"),
               threadIndex,
               unsigned(timeInSecs), unsigned((timeInSecs - unsigned(timeInSecs)) * 1000.0),
               textUpdatesObserved.GetString());

            ATLTRACE(text);
         }

         delete[] threadArgs.threadTimes;

         timeInSecs = globalTimer.Elapsed();

         ATLTRACE(_T("Total execution time is %u.%03u seconds.\n"),
            unsigned(timeInSecs), unsigned((timeInSecs - unsigned(timeInSecs)) * 1000.0));
      }
   };

} // namespace UnitTest
