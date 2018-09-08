//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file DateTime.cpp date/time class
//
#include "stdafx.h"
#include <ulib/DateTime.hpp>
#include <ulib/Exception.hpp>
#include "TimeSpanImpl.hpp"
#include "ISO8601Parser.hpp"
#pragma warning(disable: 6054) // String 'x' might not be zero-terminated.
#pragma warning(disable: 6330) // warning C6330: 'char' passed as _Param_(1) when 'unsigned char' is required in call to 'isspace'.
#pragma warning(disable: 28251) // Inconsistent annotation for 'F': this instance has no annotations
#include <boost/date_time.hpp>
#pragma warning(default: 6054)
#pragma warning(default: 6330)
#pragma warning(default: 28251)

/// \brief DateTime implementation
/// \details uses boost::date_time classes
class DateTimeImpl
{
public:
   /// ctor; initializes using special value
   explicit DateTimeImpl(boost::date_time::special_values sv = boost::date_time::not_a_date_time)
      :m_dt(sv)
   {
   }

   /// ctor; initializes using existing ptime object
   explicit DateTimeImpl(const boost::posix_time::ptime& dt)
      :m_dt(dt)
   {
   }

   /// posix time object
   boost::posix_time::ptime m_dt;
};

DateTime::DateTime()
   :m_spImpl(new DateTimeImpl)
{
}

DateTime::DateTime(unsigned int uiYear, unsigned int uiMonth, unsigned int uiDay,
   unsigned int uiHour, unsigned int uiMinute, unsigned int uiSecond, unsigned int uiMillisecond)
   : m_spImpl(new DateTimeImpl)
{
   try
   {
      m_spImpl->m_dt = boost::posix_time::ptime(
         boost::gregorian::date(USHORT(uiYear), USHORT(uiMonth), USHORT(uiDay)),
         boost::posix_time::time_duration(uiHour, uiMinute, uiSecond, uiMillisecond * 1000));
   }
   catch (...)
   {
      // illegal date/time value(s)
      m_spImpl->m_dt = boost::date_time::not_a_date_time;
   }
}

DateTime::DateTime(T_enStatus status)
   :m_spImpl(new DateTimeImpl)
{
   ATLASSERT(status == DateTime::min || status == DateTime::max);

   switch (status)
   {
   case DateTime::min: m_spImpl->m_dt = boost::date_time::min_date_time; break;
   case DateTime::max: m_spImpl->m_dt = boost::date_time::max_date_time; break;
   default:
      throw Exception(_T("invalid status in ctor"), __FILE__, __LINE__);
   }
}

DateTime::DateTime(const DateTime& dt)
   :m_spImpl(dt.m_spImpl) // copy on write
{
}

DateTime& DateTime::operator=(const DateTime& rhs)
{
   if (&rhs == this)
      return *this;

   m_spImpl = rhs.m_spImpl; // copy on write
   return *this;
}

DateTime DateTime::Now()
{
#ifdef __ANDROID__
   time_t now = time(NULL);
   struct tm tmNow = *gmtime(&now);

   return DateTime(
      tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday,
      tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);
#endif

#ifdef WIN32
   SYSTEMTIME stNow;
   GetSystemTime(&stNow);
   return DateTime(stNow.wYear, stNow.wMonth, stNow.wDay,
      stNow.wHour, stNow.wMinute, stNow.wSecond, stNow.wMilliseconds);
#endif
}

DateTime DateTime::Today()
{
   DateTime dt = DateTime::Now();

   dt.m_spImpl->m_dt = boost::posix_time::ptime(dt.m_spImpl->m_dt.date());

   return dt;
}


void DateTime::SetDateTime(unsigned int uiYear, unsigned int uiMonth, unsigned int uiDay,
   unsigned int uiHour, unsigned int uiMinute, unsigned int uiSecond, unsigned int uiMillisecond)
{
   PrepareCopy();

   try
   {
      m_spImpl->m_dt = boost::posix_time::ptime(
         boost::gregorian::date(USHORT(uiYear), USHORT(uiMonth), USHORT(uiDay)),
         boost::posix_time::time_duration(uiHour, uiMinute, uiSecond, uiMillisecond * 1000));
   }
   catch (...)
   {
      // illegal date/time span value
      m_spImpl->m_dt = boost::date_time::not_a_date_time;
   }
}

DateTime::T_enStatus DateTime::Status() const
{
   ATLASSERT(m_spImpl != NULL);

   if (m_spImpl->m_dt.is_not_a_date_time())
      return DateTime::invalid;
   else if (m_spImpl->m_dt == boost::date_time::min_date_time)
      return DateTime::min;
   else if (m_spImpl->m_dt == boost::date_time::max_date_time)
      return DateTime::max;
   else
      return DateTime::valid;
}

unsigned int DateTime::Year() const
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt.date().year();
}

unsigned int DateTime::Month() const
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt.date().month();
}

unsigned int DateTime::Day() const
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt.date().day();
}

unsigned int DateTime::Hour() const
{
   ATLASSERT(m_spImpl != NULL);
   return static_cast<int>(m_spImpl->m_dt.time_of_day().hours());
}

unsigned int DateTime::Minute() const
{
   ATLASSERT(m_spImpl != NULL);
   return static_cast<int>(m_spImpl->m_dt.time_of_day().minutes());
}

unsigned int DateTime::Second() const
{
   ATLASSERT(m_spImpl != NULL);
   return static_cast<int>(m_spImpl->m_dt.time_of_day().seconds());
}

unsigned int DateTime::Millisecond() const
{
   ATLASSERT(m_spImpl != NULL);
   return static_cast<unsigned int>(m_spImpl->m_dt.time_of_day().fractional_seconds()) / 1000;
}

unsigned int DateTime::DayOfWeek() const
{
   ATLASSERT(m_spImpl != NULL);
   // day of week returns numbers from 0..6, 0 being sunday
   return m_spImpl->m_dt.date().day_of_week().as_number();
}

unsigned int DateTime::DayOfYear() const
{
   ATLASSERT(m_spImpl != NULL);
   // day of year returns numbers from 1..366
   return m_spImpl->m_dt.date().day_of_year();
}

TimeSpan DateTime::TimeOfDay()
{
   return TimeSpan(Hour(), Minute(), Second(), Millisecond());
}

// operators

DateTime DateTime::operator+(const TimeSpan& rhs) const
{
   DateTime dt(*this);
   dt += rhs;
   return dt;
}

TimeSpan DateTime::operator-(const DateTime& rhs) const
{
   ATLASSERT(m_spImpl != NULL);

   TimeSpan span;
   span.m_spImpl->m_span = m_spImpl->m_dt - rhs.m_spImpl->m_dt;
   return span;
}

DateTime& DateTime::operator+=(const TimeSpan& span)
{
   ATLASSERT(m_spImpl != NULL);
   PrepareCopy();
   m_spImpl->m_dt += span.m_spImpl->m_span;
   return *this;
}

DateTime& DateTime::operator-=(const TimeSpan& span)
{
   ATLASSERT(m_spImpl != NULL);
   PrepareCopy();
   m_spImpl->m_dt -= span.m_spImpl->m_span;
   return *this;
}

DateTime DateTime::operator-(const TimeSpan& rhs) const
{
   DateTime dt(*this);
   dt -= rhs;
   return dt;
}

bool DateTime::operator==(const DateTime& rhs) const
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt == rhs.m_spImpl->m_dt;
}

bool DateTime::operator!=(const DateTime& rhs) const
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt != rhs.m_spImpl->m_dt;
}

bool DateTime::operator>(const DateTime& rhs) const
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt > rhs.m_spImpl->m_dt;
}

bool DateTime::operator<(const DateTime& rhs) const
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt < rhs.m_spImpl->m_dt;
}

CString DateTime::FormatISO8601(T_enISO8601Format enFormat, bool bBasic, const TimeZone& tz) const
{
   CString cszDate;
   switch (enFormat)
   {
   case formatY:  return Format(_T("%Y"), tz);
   case formatYM: return Format(bBasic ? _T("%Y%m") : _T("%Y-%m"), tz);
   case formatYMD: return Format(bBasic ? _T("%Y%m%d") : _T("%Y-%m-%d"), tz);
   case formatYMD_HM_Z:
      cszDate = Format(bBasic ? _T("%Y%m%dT%H%M") : _T("%Y-%m-%dT%H:%M"), tz);
      break;

   case formatYMD_HMS_Z:
      cszDate = Format(bBasic ? _T("%Y%m%dT%H%M%S") : _T("%Y-%m-%dT%H:%M:%S"), tz);
      break;

   case formatYMD_HMSF_Z:
   {
      cszDate = Format(bBasic ? _T("%Y%m%dT%H%M%S") : _T("%Y-%m-%dT%H:%M:%S"), tz);

      CString cszFraction;
      cszFraction.Format(_T(".%03u"), Millisecond());
      cszDate += cszFraction;
   }
   break;

   default:
      ATLASSERT(false); // invalid format
      break;
   }

   // add timezone
   if (tz.StandardName() == _T("UTC"))
      cszDate += _T("Z");
   else
   {
      TimeSpan spTimezone = tz.GetUtcOffset(*this);
      bool bNegative = spTimezone < TimeSpan(0, 0, 0, 0);

      TimeSpan spTimezoneAbs = bNegative ? -spTimezone : spTimezone;

      CString cszTimezone;
      cszTimezone.Format(bBasic ? _T("%c%02u%02u") : _T("%c%02u:%02u"),
         !bNegative ? _T('+') : _T('-'),
         spTimezoneAbs.Hours(),
         spTimezoneAbs.Minutes());

      cszDate += cszTimezone;
   }

   return cszDate;
}

CString DateTime::Format(const CString& cszFormat, const TimeZone& tz) const
{
   ATLASSERT(m_spImpl != NULL);

   if (m_spImpl->m_dt.is_not_a_date_time())
      return _T("");

   // calculate offset to UTC
   TimeSpan tsTimezone = tz.GetUtcOffset(*this);

   struct tm tmTemp = boost::posix_time::to_tm(m_spImpl->m_dt + tsTimezone.m_spImpl->m_span);

   CString cszDateTime;
   LPTSTR pszBuffer = cszDateTime.GetBufferSetLength(256);
   _tcsftime(pszBuffer, cszDateTime.GetLength(), cszFormat, &tmTemp);
   cszDateTime.ReleaseBuffer();

   return cszDateTime;
}

void DateTime::ParseISO8601(const CString& cszISO8601Timestamp) const
{
   if (cszISO8601Timestamp.IsEmpty())
   {
      m_spImpl->m_dt = boost::date_time::not_a_date_time;
      return;
   }

   ISO8601Parser p(cszISO8601Timestamp);
   if (p.IsValid())
      m_spImpl->m_dt = p.Get();
}

void DateTime::PrepareCopy()
{
   ATLASSERT(m_spImpl != NULL);

   if (!m_spImpl.unique())
      m_spImpl.reset(new DateTimeImpl(m_spImpl->m_dt));
}
