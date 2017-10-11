//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file TimeSpan.cpp time span class
//
#include "stdafx.h"
#include <ulib/TimeSpan.hpp>
#include <ulib/Exception.hpp>
#include "TimeSpanImpl.hpp"
#pragma warning(disable: 6054) // String 'x' might not be zero-terminated.
#include <boost/date_time.hpp>
#pragma warning(default: 6054)

TimeSpan::TimeSpan()
   :m_spImpl(new TimeSpanImpl)
{
}

TimeSpan::TimeSpan(int iHours, int iMins, int iSecs, int iMillisecs)
   : m_spImpl(new TimeSpanImpl)
{
   SetDateTimeSpan(iHours, iMins, iSecs, iMillisecs);
}

TimeSpan::TimeSpan(T_enStatus status)
   : m_spImpl(new TimeSpanImpl)
{
   ATLASSERT(status == TimeSpan::min || status == TimeSpan::max);

   switch (status)
   {
   case TimeSpan::min: m_spImpl->m_span = boost::date_time::min_date_time; break;
   case TimeSpan::max: m_spImpl->m_span = boost::date_time::max_date_time; break;
   default:
      throw Exception(_T("invalid status in ctor"), __FILE__, __LINE__);
   }
}

TimeSpan::TimeSpan(const TimeSpan& dt)
   :m_spImpl(dt.m_spImpl)
{
}

TimeSpan& TimeSpan::operator=(const TimeSpan& rhs)
{
   if (&rhs == this)
      return *this;

   m_spImpl = rhs.m_spImpl;
   return *this;
}

TimeSpan::T_enStatus TimeSpan::Status() const
{
   ATLASSERT(m_spImpl != nullptr);

   if (m_spImpl->m_span.is_not_a_date_time())
      return TimeSpan::invalid;
   else if (m_spImpl->m_span == boost::date_time::min_date_time)
      return TimeSpan::min;
   else if (m_spImpl->m_span == boost::date_time::max_date_time)
      return TimeSpan::max;
   else
      return TimeSpan::valid;
}

int TimeSpan::Hours() const
{
   ATLASSERT(m_spImpl != nullptr);
   return static_cast<int>(m_spImpl->m_span.hours());
}

int TimeSpan::Minutes() const
{
   ATLASSERT(m_spImpl != nullptr);
   return static_cast<int>(m_spImpl->m_span.minutes());
}

int TimeSpan::Seconds() const
{
   ATLASSERT(m_spImpl != nullptr);
   return static_cast<int>(m_spImpl->m_span.seconds());
}

int TimeSpan::Milliseconds() const
{
   ATLASSERT(m_spImpl != nullptr);
   return static_cast<int>(m_spImpl->m_span.fractional_seconds()) / 1000;
}

double TimeSpan::TotalHours() const
{
   ATLASSERT(m_spImpl != nullptr);
   return static_cast<double>(m_spImpl->m_span.ticks() / (3600 * m_spImpl->m_span.ticks_per_second()));
}

double TimeSpan::TotalMinutes() const
{
   ATLASSERT(m_spImpl != nullptr);
   return static_cast<double>(m_spImpl->m_span.ticks() / (60 * m_spImpl->m_span.ticks_per_second()));
}

double TimeSpan::TotalSeconds() const
{
   ATLASSERT(m_spImpl != nullptr);
   return static_cast<double>(m_spImpl->m_span.ticks() / (m_spImpl->m_span.ticks_per_second()));
}

double TimeSpan::TotalMilliseconds() const
{
   ATLASSERT(m_spImpl != nullptr);
   return static_cast<double>(m_spImpl->m_span.ticks() / (m_spImpl->m_span.ticks_per_second() / 1000.0));
}

TimeSpan TimeSpan::operator+(const TimeSpan& rhs) const
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(rhs.m_spImpl != NULL);

   TimeSpan dts;
   dts.m_spImpl->m_span = m_spImpl->m_span + rhs.m_spImpl->m_span;
   return dts;
}

TimeSpan TimeSpan::operator-(const TimeSpan& rhs) const
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(rhs.m_spImpl != NULL);

   TimeSpan dts;
   dts.m_spImpl->m_span = m_spImpl->m_span - rhs.m_spImpl->m_span;
   return dts;
}

TimeSpan TimeSpan::operator*(int iFactor) const
{
   ATLASSERT(m_spImpl != nullptr);

   TimeSpan dts;
   dts.m_spImpl->m_span = m_spImpl->m_span * iFactor;
   return dts;
}

TimeSpan TimeSpan::operator/(int iFactor) const
{
   ATLASSERT(m_spImpl != nullptr);

   TimeSpan dts;
   dts.m_spImpl->m_span = m_spImpl->m_span / iFactor;
   return dts;
}

TimeSpan& TimeSpan::operator+=(const TimeSpan span)
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(span.m_spImpl != NULL);

   PrepareCopy();

   m_spImpl->m_span += span.m_spImpl->m_span;
   return *this;
}

TimeSpan& TimeSpan::operator-=(const TimeSpan span)
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(span.m_spImpl != NULL);

   PrepareCopy();

   m_spImpl->m_span -= span.m_spImpl->m_span;
   return *this;
}

TimeSpan& TimeSpan::operator*=(int iFactor)
{
   ATLASSERT(m_spImpl != nullptr);

   PrepareCopy();

   m_spImpl->m_span *= iFactor;
   return *this;
}

TimeSpan& TimeSpan::operator/=(int iFactor)
{
   ATLASSERT(m_spImpl != nullptr);

   PrepareCopy();

   m_spImpl->m_span /= iFactor;
   return *this;
}

TimeSpan TimeSpan::operator-() const
{
   ATLASSERT(m_spImpl != nullptr);

   TimeSpan dts(*this);
   dts.PrepareCopy();
   dts.m_spImpl->m_span = -dts.m_spImpl->m_span;

   return dts;
}

bool TimeSpan::operator==(const TimeSpan& rhs) const
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(rhs.m_spImpl != NULL);

   return m_spImpl->m_span == rhs.m_spImpl->m_span;
}

bool TimeSpan::operator!=(const TimeSpan& rhs) const
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(rhs.m_spImpl != NULL);

   return m_spImpl->m_span != rhs.m_spImpl->m_span;
}

bool TimeSpan::operator>(const TimeSpan& rhs) const
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(rhs.m_spImpl != NULL);

   return m_spImpl->m_span > rhs.m_spImpl->m_span;
}

bool TimeSpan::operator<(const TimeSpan& rhs) const
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(rhs.m_spImpl != NULL);

   return m_spImpl->m_span < rhs.m_spImpl->m_span;
}

void TimeSpan::SetDateTimeSpan(int iHours, int iMins, int iSecs, int iMillisecs)
{
   ATLASSERT(m_spImpl != nullptr);

   PrepareCopy();

   try
   {
      // note: multiplying milliseconds with 1000 here, since resolution is in microsecs
      m_spImpl->m_span = boost::posix_time::time_duration(iHours, iMins, iSecs, iMillisecs * 1000);
   }
   catch (...)
   {
      // illegal date/time span value
      m_spImpl->m_span = boost::date_time::not_a_date_time;
   }
}

CString TimeSpan::FormatISO8601() const
{
   ATLASSERT(m_spImpl != nullptr);

   if (m_spImpl->m_span.is_not_a_date_time())
      return _T("");

   CString cszFormat(_T("P%H:%M:%S"));

   struct tm tmTemp = boost::posix_time::to_tm(m_spImpl->m_span);

   CString cszDateTime;
   LPTSTR pszBuffer = cszDateTime.GetBufferSetLength(256);
   _tcsftime(pszBuffer, cszDateTime.GetLength(), cszFormat, &tmTemp);
   cszDateTime.ReleaseBuffer();

   return cszDateTime;
}

void TimeSpan::PrepareCopy()
{
   ATLASSERT(m_spImpl != nullptr);

   if (!m_spImpl.unique())
      m_spImpl.reset(new TimeSpanImpl(m_spImpl->m_span));
}
