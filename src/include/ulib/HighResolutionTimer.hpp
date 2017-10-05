//
// ulib - a collection of useful classes
// Copyright (C) 2008-2014,2017 Michael Fink
//
/// \file HighResolutionTimer.hpp High resolution timer class
//
#pragma once

// includes

/// \brief High resolution timer class
/// \details Has a high precision. Has identical interface as class Timer.
/// Can be used in TraceOutputStopwatch.
class HighResolutionTimer
{
public:
   /// ctor
   HighResolutionTimer();

   /// starts timer; timer must be stopped or reset
   void Start();

   /// stops timer; timer must be started
   void Stop();

   /// resets timer; timer must be stopped
   void Reset();

   /// restarts timer by resetting and starting again
   void Restart();

   /// returns elapsed time since Start() was called, in seconds
   double Elapsed() const;

   /// returns total elapsed time in seconds
   double TotalElapsed() const;

   /// returns if timer is running
   bool IsStarted() const { return m_isStarted; }

private:
   /// indicates if timer is started
   bool m_isStarted;

   /// start time
   LARGE_INTEGER m_startTime;

   /// already elapsed time
   LARGE_INTEGER m_totalElapsed;
};
