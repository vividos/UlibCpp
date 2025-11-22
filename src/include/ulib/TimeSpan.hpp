//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017,2020,2025 Michael Fink
//
/// \file TimeSpan.hpp time span class
//
#pragma once

#include <chrono>

/// represents a relative time span
class TimeSpan
{
public:
   /// ctor, optionally taking a milliseconds duration
   TimeSpan(std::chrono::milliseconds span = std::chrono::milliseconds::zero())
      :m_span(span)
   {
   }

   /// ctor; takes date/time span components
   TimeSpan(int hours, int minutes, int seconds, int milliseconds = 0)
   {
      m_span =
         std::chrono::hours(hours) +
         std::chrono::minutes(minutes) +
         std::chrono::seconds(seconds) +
         std::chrono::milliseconds(milliseconds);
   }

   // properties

   /// component hours in span (-23 to 23)
   int Hours() const
   {
      auto h = std::chrono::duration_cast<std::chrono::hours>(m_span);
      return h.count() % 24;
   }

   /// component minutes in span (-59 to 59)
   int Minutes() const
   {
      auto m = std::chrono::duration_cast<std::chrono::minutes>(m_span);
      return m.count() % 60;
   }

   /// component seconds in span (-59 to 59)
   int Seconds() const
   {
      auto s = std::chrono::duration_cast<std::chrono::seconds>(m_span);
      return s.count() % 60;
   }

   /// component milliseconds in span (-999 to 999)
   int Milliseconds() const
   {
      return m_span.count() % 1000;
   }

   /// span in hours (about -8.77e7 to 8.77e6)
   double TotalHours() const
   {
      return m_span.count() / 1000.0 / 60.0 / 60.0;
   }

   /// span in minutes (about -5.26e9 to 5.26e9)
   double TotalMinutes() const
   {
      return m_span.count() / 1000.0 / 60.0;
   }

   /// span in seconds (about -3.16e11 to 3.16e11)
   double TotalSeconds() const
   {
      return m_span.count() / 1000.0;
   }

   /// span in milliseconds
   double TotalMilliseconds() const
   {
      return static_cast<double>(m_span.count());
   }

   // operators

   /// add operator
   TimeSpan operator+(const TimeSpan& rhs) const
   {
      return TimeSpan(m_span + rhs.m_span);
   }

   /// subtract operator
   TimeSpan operator-(const TimeSpan& rhs) const
   {
      return TimeSpan(m_span - rhs.m_span);
   }

   /// multiply by scalar operator
   TimeSpan operator*(int factor) const
   {
      return TimeSpan(factor * m_span);
   }

   /// divide by scalar operator
   TimeSpan operator/(int factor) const
   {
      return TimeSpan(m_span / factor);
   }

   /// add inplace operator
   TimeSpan& operator+=(const TimeSpan span)
   {
      m_span += span.m_span;
      return *this;
   }

   /// subtract inplace operator
   TimeSpan& operator-=(const TimeSpan span)
   {
      m_span -= span.m_span;
      return *this;
   }

   /// multiply by scalar inplace operator
   TimeSpan& operator*=(int factor)
   {
      m_span *= factor;
      return *this;
   }

   /// divide by scalar inplace operator
   TimeSpan& operator/=(int factor)
   {
      m_span /= factor;
      return *this;
   }

   /// negate operator
   TimeSpan operator-() const
   {
      return TimeSpan(-m_span);
   }

   /// equal operator
   bool operator==(const TimeSpan& rhs) const
   {
      return m_span == rhs.m_span;
   }

   /// unequal operator
   bool operator!=(const TimeSpan& rhs) const
   {
      return m_span != rhs.m_span;
   }

   /// greater operator
   bool operator>(const TimeSpan& rhs) const
   {
      return m_span > rhs.m_span;
   }

   /// less operator
   bool operator<(const TimeSpan& rhs) const
   {
      return m_span < rhs.m_span;
   }

   /// greater or equal operator
   bool operator<=(const TimeSpan& rhs) const
   {
      return !operator>(rhs);
   }

   /// less or equal operator
   bool operator>=(const TimeSpan& rhs) const
   {
      return !operator<(rhs);
   }

   /// sets time span components
   void SetDateTimeSpan(int hours, int minutes, int seconds, int milliseconds)
   {
      m_span =
         std::chrono::hours(hours) +
         std::chrono::minutes(minutes) +
         std::chrono::seconds(seconds) +
         std::chrono::milliseconds(milliseconds);
   }

   /// time span format
   enum T_enTimeSpanFormat
   {
      formatHMS,     ///< "hh:mm:ss" format
      formatISO8601, ///< ISO 8601, "PTxxHxxMxxS" format; only positive spans as duration
   };

   /// formats time span using specified format
   CString Format(T_enTimeSpanFormat format = formatHMS) const
   {
      switch (format)
      {
      case TimeSpan::formatHMS:
         return CString{
            std::format(
               "{:%H:%M}:{:02}",
               m_span,
               std::abs(Seconds())).c_str() };

      case TimeSpan::formatISO8601:
         return CString{
            std::format(
               "PT{}H{}M{}S",
               std::abs(Hours()),
               std::abs(Minutes()),
               std::abs(Seconds())).c_str() };

      default:
         ATLASSERT(false); // invalid format type
         return CString{};
      }
   }

private:
   friend class DateTime;

   // time span in ms resolution
   std::chrono::milliseconds m_span;
};
