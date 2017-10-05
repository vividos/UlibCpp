//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2013,2016,2017 Michael Fink
//
/// \file Timer.hpp Timer class
//
#pragma once

#pragma warning(push)
#pragma warning(disable: 28159) // Consider using 'GetTickCount64' instead of 'GetTickCount'. Reason: GetTickCount overflows roughly every 49 days.  Code that does not take that into account can loop indefinitely.  GetTickCount64 operates on 64 bit values and does not have that problem

/// \brief Timer class
/// \details Has a precision of about 15 ms. Has identical interface as class
/// HighResolutionTimer. Can be used in TraceOutputStopwatch.
class Timer
{
public:
   /// ctor
   Timer()
      :m_isStarted(false),
      m_tickStart(0),
      m_totalElapsed(0)
   {
   }

   /// starts timer; timer must be stopped or reset
   void Start()
   {
      ATLASSERT(m_isStarted == false);
      m_tickStart = GetTickCount();
      m_isStarted = true;
   }

   /// stops timer; timer must be started
   void Stop()
   {
      ATLASSERT(m_isStarted == true);

      DWORD tickEnd = GetTickCount();
      m_totalElapsed += (tickEnd - m_tickStart);
      m_isStarted = false;
   }

   /// resets timer; timer must be stopped
   void Reset()
   {
      ATLASSERT(m_isStarted == false); // must be stopped when calling Reset()
      m_totalElapsed = 0;
   }

   /// restarts timer by resetting and starting again
   void Restart()
   {
      // can either be stopped or started when restarting
      if (m_isStarted)
         Stop();
      Reset();
      Start();
   }

   /// returns elapsed time since Start() was called, in seconds
   double Elapsed() const
   {
      if (!m_isStarted)
         return 0.0; // no time elapsed, since timer isn't started

      DWORD tickNow = GetTickCount();
      return 0.001 * (tickNow - m_tickStart);
   }

   /// returns total elapsed time in seconds
   double TotalElapsed() const
   {
      DWORD tickNow = GetTickCount();
      return 0.001 * (m_totalElapsed + (m_isStarted ? (tickNow - m_tickStart) : 0));
   }

   /// returns if timer is running
   bool IsStarted() const { return m_isStarted; }

private:
   /// indicates if timer is started
   bool m_isStarted;

   /// tick count of start
   DWORD m_tickStart;

   /// number of ticks already elapsed
   DWORD m_totalElapsed;
};

#pragma warning(pop)
