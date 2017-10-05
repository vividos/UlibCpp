//
// ulib - a collection of useful classes
// Copyright (C) 2008-2014,2017 Michael Fink
//
/// \file TraceOutputStopwatch.hpp Stopwatch class that does trace output
//
#pragma once

/// \brief Stopwatch that does trace output
/// \tparam T timer class to use; may be Timer or HighResolutionTimer
template <typename T>
class TraceOutputStopwatch
{
public:
   /// ctor; starts timer
   TraceOutputStopwatch(LPCTSTR timerName)
      :m_timerName(timerName)
   {
      ATLTRACE(_T("Timer %s START\n"), timerName);
      m_timer.Start();
   }

   /// dtor; stops timer and does output
   ~TraceOutputStopwatch()
   {
      double elapsed = m_timer.TotalElapsed();
      ATLTRACE(_T("Timer %s STOP: "), m_timerName);
      if (elapsed < 0.001)
         ATLTRACE(_T("0.%03u ms"), unsigned(elapsed * 1000.0 * 1000.0));
      else
         if (elapsed < 1.0)
            ATLTRACE(_T("0.%03u s"), unsigned(elapsed * 1000.0));
         else
            ATLTRACE(_T("%u.%03u s"), unsigned(elapsed), unsigned((elapsed - unsigned(elapsed)) * 1000.0));

      ATLTRACE(_T("\n"));
   }

private:
   LPCTSTR m_timerName; ///< timer name
   T m_timer;           ///< timer class
};
