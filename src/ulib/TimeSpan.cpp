//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017,2019,2020 Michael Fink
//
/// \file TimeSpan.cpp time span class
//
#include "stdafx.h"
#include <ulib/TimeSpan.hpp>
#include <ulib/Exception.hpp>
#include "TimeSpanImpl.hpp"
#include <ulib/config/BoostDateTime.hpp>

TimeSpan::TimeSpan()
   :m_spImpl(new TimeSpanImpl)
{
}

TimeSpan::TimeSpan(int hours, int minutes, int seconds, int millseconds)
   : m_spImpl(new TimeSpanImpl)
{
   SetDateTimeSpan(hours, minutes, seconds, millseconds);
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

TimeSpan::TimeSpan(TimeSpan&& dt) noexcept
   :m_spImpl(std::move(dt.m_spImpl))
{
}

TimeSpan& TimeSpan::operator=(const TimeSpan& rhs)
{
   if (&rhs == this)
      return *this;

   m_spImpl = rhs.m_spImpl;
   return *this;
}

TimeSpan& TimeSpan::operator=(TimeSpan&& rhs) noexcept
{
   m_spImpl = std::move(rhs.m_spImpl);
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

TimeSpan TimeSpan::operator*(int factor) const
{
   ATLASSERT(m_spImpl != nullptr);

   TimeSpan dts;
   dts.m_spImpl->m_span = m_spImpl->m_span * factor;
   return dts;
}

TimeSpan TimeSpan::operator/(int factor) const
{
   ATLASSERT(m_spImpl != nullptr);

   TimeSpan dts;
   dts.m_spImpl->m_span = m_spImpl->m_span / factor;
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

TimeSpan& TimeSpan::operator*=(int factor)
{
   ATLASSERT(m_spImpl != nullptr);

   PrepareCopy();

   m_spImpl->m_span *= factor;
   return *this;
}

TimeSpan& TimeSpan::operator/=(int factor)
{
   ATLASSERT(m_spImpl != nullptr);

   PrepareCopy();

   m_spImpl->m_span /= factor;
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

void TimeSpan::SetDateTimeSpan(int hours, int minutes, int seconds, int millseconds)
{
   ATLASSERT(m_spImpl != nullptr);

   PrepareCopy();

   try
   {
      // note: multiplying milliseconds with 1000 here, since resolution is in microsecs
      m_spImpl->m_span = boost::posix_time::time_duration(hours, minutes, seconds,
         boost::posix_time::time_duration::fractional_seconds_type(millseconds) * 1000);
   }
   catch (...)
   {
      // illegal date/time span value
      m_spImpl->m_span = boost::date_time::not_a_date_time;
   }
}

CString TimeSpan::Format(TimeSpan::T_enTimeSpanFormat format) const
{
   ATLASSERT(m_spImpl != nullptr);

   if (m_spImpl->m_span.is_not_a_date_time())
      return CString();

   if (m_spImpl->m_span == boost::date_time::min_date_time)
      return _T("min");

   if (m_spImpl->m_span == boost::date_time::max_date_time)
      return _T("max");

   switch (format)
   {
   case TimeSpan::formatHMS:
      return (m_spImpl->m_span.is_negative() ? _T("-") : _T("")) + Format(_T("%H:%M:%S"));

   case TimeSpan::formatISO8601:
      return Format(_T("PT%HH%MM%SS"));

   default:
      ATLASSERT(false); // invalid format type
      break;
   }

   return CString();
}

CString TimeSpan::Format(LPCTSTR format) const
{
   ATLASSERT(m_spImpl != nullptr);

   if (m_spImpl->m_span.is_not_a_date_time())
      return CString();

   if (m_spImpl->m_span == boost::date_time::min_date_time)
      return _T("min");

   if (m_spImpl->m_span == boost::date_time::max_date_time)
      return _T("max");

   struct tm tmTemp = boost::posix_time::to_tm(m_spImpl->m_span);

   CString text;
   LPTSTR buffer = text.GetBufferSetLength(256);
   _tcsftime(buffer, text.GetLength(), format, &tmTemp);
   text.ReleaseBuffer();

   return text;
}

CString TimeSpan::FormatISO8601() const
{
   Format(T_enTimeSpanFormat::formatHMS);
}

void TimeSpan::PrepareCopy()
{
   ATLASSERT(m_spImpl != nullptr);

   if (m_spImpl.use_count() > 1)
      m_spImpl.reset(new TimeSpanImpl(m_spImpl->m_span));
}
