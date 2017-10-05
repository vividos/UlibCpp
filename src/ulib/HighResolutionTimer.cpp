//
// ulib - a collection of useful classes
// Copyright (C) 2008-2014,2017 Michael Fink
//
/// \file HighResolutionTimer.cpp High resolution timer class
//

// includes
#include "stdafx.h"
#include <ulib/HighResolutionTimer.hpp>

/// returns performance counter frequency in counts / seconds
static LONGLONG GetPerformanceFrequency()
{
   static LARGE_INTEGER s_lFrequency = { 0 };
   if (s_lFrequency.QuadPart == 0LL)
   {
      ATLVERIFY(TRUE == ::QueryPerformanceFrequency(&s_lFrequency));
   }

   return s_lFrequency.QuadPart;
}

HighResolutionTimer::HighResolutionTimer()
   :m_isStarted(false)
{
   m_startTime.QuadPart = 0LL;
   m_totalElapsed.QuadPart = 0LL;
}

void HighResolutionTimer::Start() throw()
{
   ATLASSERT(m_isStarted == false);
   ATLVERIFY(TRUE == ::QueryPerformanceCounter(&m_startTime));
   m_isStarted = true;
}

void HighResolutionTimer::Stop() throw()
{
   ATLASSERT(m_isStarted == true);

   LARGE_INTEGER lCountNow = { 0 };
   ATLVERIFY(TRUE == ::QueryPerformanceCounter(&lCountNow));
   m_totalElapsed.QuadPart += (lCountNow.QuadPart - m_startTime.QuadPart);
   m_isStarted = false;
}

void HighResolutionTimer::Reset() throw()
{
   ATLASSERT(m_isStarted == false); // must be stopped when calling Reset()
   m_totalElapsed.QuadPart = 0;
}

void HighResolutionTimer::Restart() throw()
{
   // can either be stopped or started when restarting
   if (m_isStarted)
      Stop();
   Reset();
   Start();
}

double HighResolutionTimer::Elapsed() const throw()
{
   if (!m_isStarted)
      return 0.0; // no time elapsed, since timer isn't started

   LARGE_INTEGER lCountNow = { 0 };
   ATLVERIFY(TRUE == ::QueryPerformanceCounter(&lCountNow));
   return static_cast<double>(lCountNow.QuadPart - m_startTime.QuadPart) / GetPerformanceFrequency();
}

double HighResolutionTimer::TotalElapsed() const throw()
{
   LARGE_INTEGER lCountNow = { 0 };
   ATLVERIFY(TRUE == ::QueryPerformanceCounter(&lCountNow));
   return static_cast<double>(
      m_totalElapsed.QuadPart + (m_isStarted ? (lCountNow.QuadPart - m_startTime.QuadPart) : 0LL)) / GetPerformanceFrequency();
}
