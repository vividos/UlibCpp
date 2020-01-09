//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file DateTime.hpp date/time class
//
#pragma once

#include <ulib/TimeSpan.hpp>
#include <ulib/TimeZone.hpp>
#include <memory>

/// represents a date/time point
class DateTime
{
public:
   /// date/time status values
   enum T_enStatus { valid = 0, invalid, min, max };

   /// default ctor
   DateTime();

   /// ctor; takes date/time components
   DateTime(unsigned int uiYear, unsigned int uiMonth, unsigned int uiDay,
      unsigned int uiHour, unsigned int uiMinute, unsigned int uiSecond,
      unsigned int uiMillisecond = 0);

   /// ctor; initialize with min or max status
   explicit DateTime(T_enStatus status);

   /// copy ctor
   DateTime(const DateTime& dt);

   /// move ctor
   DateTime(DateTime&& dt) noexcept;

   /// dtor
   ~DateTime() {}

   /// copy assignment operator
   DateTime& operator=(const DateTime& rhs);

   /// move assignment operator
   DateTime& operator=(DateTime&& rhs) noexcept;

   /// returns current date/time
   static DateTime Now();

   /// returns current date with time part set to 00:00:00
   static DateTime Today();

   /// returns max time value
   static DateTime MaxValue() { return DateTime(DateTime::max); }

   /// returns min time value
   static DateTime MinValue() { return DateTime(DateTime::min); }

   /// sets date/time components
   void SetDateTime(unsigned int uiYear, unsigned int uiMonth, unsigned int uiDay,
      unsigned int uiHour, unsigned int uiMinute, unsigned int uiSecond,
      unsigned int uiMillisecond = 0);

   // properties

   /// returns date/time status
   T_enStatus Status() const;

   /// returns year
   unsigned int Year() const;
   /// month of year; 1 = january
   unsigned int Month() const;
   /// day of month (1-31)
   unsigned int Day() const;

   /// hour in day (0-23)
   unsigned int Hour() const;
   /// minute in hour (0-59)
   unsigned int Minute() const;
   /// second in minute (0-59)
   unsigned int Second() const;
   /// millisecond in second (0-999)
   unsigned int Millisecond() const;

   /// day of week; 0: sunday; ... 6: saturday
   unsigned int DayOfWeek() const;
   /// day of year; 1-based
   unsigned int DayOfYear() const;
   /// returns time part of date
   TimeSpan TimeOfDay();

   // operators

   DateTime operator+(const TimeSpan& rhs) const; ///< add operator
   DateTime operator-(const TimeSpan& rhs) const; ///< subtract operator
   DateTime& operator+=(const TimeSpan& span);    ///< add inplace operator
   DateTime& operator-=(const TimeSpan& span);    ///< subtract inplace operator
   TimeSpan operator-(const DateTime& rhs) const; ///< subtracts two date/times

   bool operator==(const DateTime& rhs) const;  ///< equal operator
   bool operator!=(const DateTime& rhs) const;  ///< unequal operator
   bool operator>(const DateTime& rhs) const;   ///< greater operator
   bool operator<(const DateTime& rhs) const;   ///< less operator
   bool operator<=(const DateTime& rhs) const { return !operator>(rhs); } ///< greater or equal operator
   bool operator>=(const DateTime& rhs) const { return !operator<(rhs); } ///< less or equal operator

   /// ISO 8601 format
   enum T_enISO8601Format
   {
      formatY,       ///< year only
      formatYM,      ///< year and month
      formatYMD,     ///< year and month
      formatYMD_HM_Z,   ///< year, month, day, hour, minute and timezone offset
      formatYMD_HMS_Z,  ///<year, month, day, hour, minute, second and timezone offset
      formatYMD_HMSF_Z, ///< full date/time with fraction and timezone offset
   };

   /// formats date/time using ISO 8601 format
   CString FormatISO8601(T_enISO8601Format enFormat = formatYMD_HMS_Z, bool bBasic = false,
      const TimeZone& tz = TimeZone::System()) const;

   /// formats date/time with given format, see _tcsftime
   CString Format(const CString& cszFormat, const TimeZone& tz = TimeZone::System()) const;

   /// parses ISO 8601 formatted date/time
   void ParseISO8601(const CString& cszISO8601Timestamp) const;

private:
   /// prepares another copy of the object when date/time value is about to be modified
   void PrepareCopy();

private:
   /// implementation
   std::shared_ptr<class DateTimeImpl> m_spImpl;
};
