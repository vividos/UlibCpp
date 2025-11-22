//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017,2020,2025 Michael Fink
//
/// \file DateTime.hpp date/time class
//
#pragma once

#include <ulib/TimeSpan.hpp>
#include <ulib/TimeZone.hpp>
#include <chrono>

/// represents a date/time point
class DateTime
{
public:
   /// type of time point used to represent the date and time
   typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> time_point_type;

   /// date/time status values
   enum class T_enStatus : uint8_t { valid = 0, invalid, min, max };

   /// default ctor; initialized with invalid status
   DateTime()
      :m_status(T_enStatus::invalid)
   {
   }

   /// <summary>
   /// ctor; takes a time point object
   /// </summary>
   /// <param name="timePoint">time point</param>
   DateTime(time_point_type timePoint)
      :m_timePoint(timePoint),
      m_status(T_enStatus::valid)
   {
   }

   /// ctor; takes date/time components
   DateTime(unsigned int year, unsigned int month, unsigned int day,
      unsigned int hour, unsigned int minute, unsigned int second,
      unsigned int millisecond = 0)
      :m_status(T_enStatus::invalid)
   {
      SetDateTime(year, month, day, hour, minute, second, millisecond);
   }

   /// ctor; initialize with min or max status only
   explicit DateTime(T_enStatus status)
      :m_status(status)
   {
      ATLASSERT(status == DateTime::T_enStatus::min || status == DateTime::T_enStatus::max);
   }

   /// returns current date/time in local time
   static DateTime Now()
   {
      return DateTime(
         std::chrono::time_point_cast<time_point_type::duration>(
            std::chrono::system_clock::now()));
   }

   /// returns current date with time part set to 00:00:00
   static DateTime Today()
   {
      auto now = Now();
      return now - now.TimeOfDay();
   }

   /// returns max time value
   static DateTime MaxValue()
   {
      return DateTime(DateTime::T_enStatus::max);
   }

   /// returns min time value
   static DateTime MinValue()
   {
      return DateTime(DateTime::T_enStatus::min);
   }

   /// sets date/time components
   void SetDateTime(unsigned int year, unsigned int month, unsigned int day,
      unsigned int hour, unsigned int minute, unsigned int second,
      unsigned int millisecond = 0)
   {
      m_status = T_enStatus::valid;

      m_timePoint = std::chrono::sys_days{ std::chrono::year(year) / month / day } +
         std::chrono::hours(hour) +
         std::chrono::minutes(minute) +
         std::chrono::seconds(second) +
         std::chrono::milliseconds(millisecond);
   }

   // properties

   /// returns date/time status
   T_enStatus Status() const { return m_status; }

   /// returns year
   unsigned int Year() const
   {
      auto dayPoint = std::chrono::floor<std::chrono::days>(m_timePoint);
      auto ymd = std::chrono::year_month_day{ dayPoint };
      return static_cast<unsigned int>(
         static_cast<int>(
            ymd.year()));
   }

   /// month of year; 1 = january
   unsigned int Month() const
   {
      auto dayPoint = std::chrono::floor<std::chrono::days>(m_timePoint);
      auto ymd = std::chrono::year_month_day{ dayPoint };
      return static_cast<unsigned int>(ymd.month());
   }

   /// day of month (1-31)
   unsigned int Day() const
   {
      auto dayPoint = std::chrono::floor<std::chrono::days>(m_timePoint);
      auto ymd = std::chrono::year_month_day{ dayPoint };
      return static_cast<unsigned int>(ymd.day());
   }

   /// hour in day (0-23)
   unsigned int Hour() const
   {
      return static_cast<unsigned int>(TimeOfDay().Hours());
   }

   /// minute in hour (0-59)
   unsigned int Minute() const
   {
      return static_cast<unsigned int>(TimeOfDay().Minutes());
   }

   /// second in minute (0-59)
   unsigned int Second() const
   {
      return static_cast<unsigned int>(TimeOfDay().Seconds());
   }

   /// millisecond in second (0-999)
   unsigned int Millisecond() const
   {
      return static_cast<unsigned int>(TimeOfDay().Milliseconds());
   }

   /// returns day of week, with numbers from 0..6, and 0 being sunday
   unsigned int DayOfWeek() const
   {
      auto dayPoint = std::chrono::floor<std::chrono::days>(m_timePoint);
      std::chrono::weekday wd{ dayPoint };
      return wd.c_encoding();
   }

   /// day of year; 1-based, numbers from 1..366
   unsigned int DayOfYear() const
   {
      auto dayPoint = std::chrono::floor<std::chrono::days>(m_timePoint);
      auto ymd = std::chrono::year_month_day{ dayPoint };
      auto year = ymd.year();
      auto year_day = dayPoint - std::chrono::sys_days{ year / std::chrono::January / 0 };
      return year_day.count();
   }

   /// returns time part of date
   TimeSpan TimeOfDay() const
   {
      auto dayPoint = std::chrono::floor<std::chrono::days>(m_timePoint);
      std::chrono::hh_mm_ss time{ std::chrono::floor<std::chrono::milliseconds>(m_timePoint - dayPoint) };
      return TimeSpan(time.to_duration());
   }

   // operators

   /// add operator
   DateTime operator+(const TimeSpan& rhs) const
   {
      return DateTime(m_timePoint + rhs.m_span);
   }

   /// subtract operator
   DateTime operator-(const TimeSpan& rhs) const
   {
      return DateTime(m_timePoint - rhs.m_span);
   }

   /// add inplace operator
   DateTime& operator+=(const TimeSpan& span)
   {
      m_timePoint += span.m_span;
      return *this;
   }

   /// subtract inplace operator
   DateTime& operator-=(const TimeSpan& span)
   {
      m_timePoint -= span.m_span;
      return *this;
   }

   /// subtracts two date/times
   TimeSpan operator-(const DateTime& rhs) const
   {
      auto duration = m_timePoint - rhs.m_timePoint;
      return TimeSpan(
         std::chrono::duration_cast<std::chrono::milliseconds>(duration));
   }

   /// equal operator
   bool operator==(const DateTime& rhs) const
   {
      return m_status == rhs.m_status &&
         m_timePoint == rhs.m_timePoint;
   }

   /// unequal operator
   bool operator!=(const DateTime& rhs) const
   {
      return !operator==(rhs);
   }

   // spaceship comparison operator
   friend constexpr auto operator<=>(const DateTime& lhs, const DateTime& rhs) noexcept
   {
      // same status, but not valid?
      if (lhs.m_status == rhs.m_status && lhs.m_status != T_enStatus::valid)
         return std::strong_ordering::equal;

      // min or max status?
      if (lhs.m_status == T_enStatus::min || rhs.m_status == T_enStatus::max)
         return std::strong_ordering::less;
      if (lhs.m_status == T_enStatus::max || rhs.m_status == T_enStatus::min)
         return std::strong_ordering::greater;

      // compare actual time points
      return lhs.m_timePoint <=> rhs.m_timePoint;
   }

   /// ISO 8601 format
   enum T_enISO8601Format
   {
      formatY,       ///< year only
      formatYM,      ///< year and month
      formatYMD,     ///< year and month
      formatYMD_HM_Z,   ///< year, month, day, hour, minute and timezone offset
      formatYMD_HMS_Z,  ///< year, month, day, hour, minute, second and timezone offset
      formatYMD_HMSF_Z, ///< full date/time with fraction and timezone offset
   };

   /// formats date/time using ISO 8601 format
   CString FormatISO8601(T_enISO8601Format format = formatYMD_HMS_Z, bool basic = false,
      const TimeZone& tz = TimeZone::System()) const;

   /// formats date/time with given format, see _tcsftime
   CString Format(const CString& format, const TimeZone& tz = TimeZone::System()) const;

   /// parses ISO 8601 formatted date/time
   void ParseISO8601(const CString& iso8601Timestamp);

private:
   /// time point
   time_point_type m_timePoint;

   /// status of date time oject
   T_enStatus m_status;
};
