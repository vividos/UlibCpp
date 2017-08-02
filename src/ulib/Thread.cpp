//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2013,2017 Michael Fink
//
/// \file Thread.cpp Thread class
//

#include "stdafx.h"
#include "Thread.hpp"

#ifdef WIN32

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

void Thread::SetName(LPCSTR pszThreadName, DWORD dwThreadId)
{
#ifdef WIN32
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = pszThreadName;
   info.dwThreadID = dwThreadId;
   info.dwFlags = 0;

   __try
   {
      RaiseException(MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info);
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
      // ignore exception
   }
#endif // WIN32
}
