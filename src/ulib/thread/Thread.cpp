//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2013,2017,2019,2020 Michael Fink
//
/// \file Thread.cpp Thread class
//

#include "stdafx.h"
#include <ulib/thread/Thread.hpp>

#ifdef _WIN32

#ifndef MS_VC_EXCEPTION
/// exception code for visual studio functions
#define MS_VC_EXCEPTION 0x406D1388
#endif

#pragma pack(push, 8)
/// thread name info struct
typedef struct tagTHREADNAME_INFO
{
   DWORD dwType;     ///< Must be 0x1000.
   LPCSTR szName;    ///< Pointer to name (in user addr space).
   DWORD dwThreadID; ///< Thread ID (-1=caller thread).
   DWORD dwFlags;    ///< Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

#endif // WIN32

void Thread::SetName(LPCSTR threadName, DWORD threadId)
{
#ifdef _WIN32
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = threadName;
   info.dwThreadID = threadId;
   info.dwFlags = 0;

#pragma warning(disable: 6320) // Exception-filter expression is the constant EXCEPTION_EXECUTE_HANDLER. This might mask exceptions that were not intended to be handled.
#pragma warning(disable: 6322) // Empty _except block.
   __try
   {
      RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
   }
   __except (EXCEPTION_EXECUTE_HANDLER)
   {
      // ignore exception
   }
#pragma warning(default: 6320)
#pragma warning(default: 6322)
#endif // WIN32
}

DWORD Thread::CurrentId()
{
#if defined(WIN32)
   return ::GetCurrentThreadId();
#elif defined(__ANDROID__)
   return static_cast<DWORD>(pthread_self());
#endif
}
