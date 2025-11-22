//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017,2019,2020,2025 Michael Fink
//
/// \file DateTime.cpp date/time class
//
#include "stdafx.h"
#include <ulib/DateTime.hpp>

CString DateTime::FormatISO8601(DateTime::T_enISO8601Format enFormat, bool basic, const TimeZone& tz) const
{
   CString date;
   switch (enFormat)
   {
   case formatY:  return Format(_T("%Y"), tz);
   case formatYM: return Format(basic ? _T("%Y%m") : _T("%Y-%m"), tz);
   case formatYMD: return Format(basic ? _T("%Y%m%d") : _T("%Y-%m-%d"), tz);
   case formatYMD_HM_Z:
      date = Format(basic ? _T("%Y%m%dT%H%M") : _T("%Y-%m-%dT%H:%M"), tz);
      break;

   case formatYMD_HMS_Z:
      date = Format(basic ? _T("%Y%m%dT%H%M%S") : _T("%Y-%m-%dT%H:%M:%S"), tz);
      break;

   case formatYMD_HMSF_Z:
   {
      date = Format(basic ? _T("%Y%m%dT%H%M%S") : _T("%Y-%m-%dT%H:%M:%S"), tz);

      CString fraction;
      fraction.Format(_T(".%03u"), Millisecond());
      date += fraction;
   }
   break;

   default:
      ATLASSERT(false); // invalid format
      break;
   }

   // add timezone
   if (tz.StandardName() == _T("UTC"))
      date += _T("Z");
   else
   {
      TimeSpan spanTimezone = tz.GetUtcOffset(*this);
      bool isNegative = spanTimezone < TimeSpan(0, 0, 0, 0);

      TimeSpan spanTimezoneAbs = isNegative ? -spanTimezone : spanTimezone;

      CString cszTimezone;
      cszTimezone.Format(basic ? _T("%c%02u%02u") : _T("%c%02u:%02u"),
         !isNegative ? _T('+') : _T('-'),
         spanTimezoneAbs.Hours(),
         spanTimezoneAbs.Minutes());

      date += cszTimezone;
   }

   return date;
}

CString DateTime::Format(const CString& format, const TimeZone& tz) const
{
   if (m_status == T_enStatus::invalid)
      return _T("");

   // calculate offset to UTC
   TimeSpan spanTimezone = tz.GetUtcOffset(*this);

   time_t tt = std::chrono::system_clock::to_time_t(
         m_timePoint + spanTimezone.m_span);

   tm tmTemp = { 0 };
   gmtime_s(&tmTemp, &tt);

   CString dateTimeText;
   LPTSTR buffer = dateTimeText.GetBufferSetLength(256);
   _tcsftime(buffer, dateTimeText.GetLength(), format, &tmTemp);
   dateTimeText.ReleaseBuffer();

   return dateTimeText;
}

void DateTime::ParseISO8601(const CString& iso8601Timestamp)
{
   if (iso8601Timestamp.IsEmpty())
   {
      m_status = T_enStatus::invalid;
      return;
   }

   std::string text{ CStringA{iso8601Timestamp} };
   std::istringstream in{ text };
   std::chrono::sys_time<std::chrono::milliseconds> tp;
   in >> std::chrono::parse("%FT%TZ", tp);
   if (in.fail())
   {
      in.clear();
      in.exceptions(std::ios::failbit);
      in.str(text);
      in >> std::chrono::parse("%FT%T%Ez", tp);
   }

   m_timePoint = tp;
}
