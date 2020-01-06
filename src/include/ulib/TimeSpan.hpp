//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017,2020 Michael Fink
//
/// \file TimeSpan.hpp time span class
//
#pragma once

#include <memory>

/// represents a relative time span
class TimeSpan
{
public:
   /// date/time span status values
   enum T_enStatus { valid = 0, invalid, min, max };

   /// default ctor
   TimeSpan();

   /// ctor; takes date/time span components
   TimeSpan(int iHours, int iMins, int iSecs, int iMillisecs = 0);

   /// ctor; initialize with min or max status
   explicit TimeSpan(T_enStatus status);

   /// copy ctor
   TimeSpan(const TimeSpan& dt);

   /// move ctor
   TimeSpan(TimeSpan&& dt) noexcept;

   /// copy assignment operator
   TimeSpan& operator=(const TimeSpan& rhs);

   /// move assignment operator
   TimeSpan& operator=(TimeSpan&& rhs) noexcept;

   // properties

   /// returns date/time status
   T_enStatus Status() const;

   /// component hours in span (-23 to 23)
   int Hours() const;
   /// component minutes in span (-59 to 59)
   int Minutes() const;
   /// component seconds in span (-59 to 59)
   int Seconds() const;
   /// component milliseconds in span (-999 to 999)
   int Milliseconds() const;

   /// span in hours (about -8.77e7 to 8.77e6)
   double TotalHours() const;
   /// span in minutes (about -5.26e9 to 5.26e9)
   double TotalMinutes() const;
   /// span in seconds (about -3.16e11 to 3.16e11)
   double TotalSeconds() const;
   /// span in milliseconds
   double TotalMilliseconds() const;

   // operators
   TimeSpan operator+(const TimeSpan& rhs) const; ///< add operator
   TimeSpan operator-(const TimeSpan& rhs) const; ///< subtract operator
   TimeSpan operator*(int iFactor) const;          ///< multiply by scalar operator
   TimeSpan operator/(int iFactor) const;          ///< divide by scalar operator
   TimeSpan& operator+=(const TimeSpan span);     ///< add inplace operator
   TimeSpan& operator-=(const TimeSpan span);     ///< subtract inplace operator
   TimeSpan& operator*=(int iFactor);              ///< multiply by scalar inplace operator
   TimeSpan& operator/=(int iFactor);              ///< divide by scalar inplace operator
   TimeSpan operator-() const;                     ///< subtract two time spans operator

   bool operator==(const TimeSpan& rhs) const;     ///< equal operator
   bool operator!=(const TimeSpan& rhs) const;     ///< unequal operator
   bool operator>(const TimeSpan& rhs) const;      ///< greater operator
   bool operator<(const TimeSpan& rhs) const;      ///< less operator
   bool operator<=(const TimeSpan& rhs) const { return !operator>(rhs); }  ///< greater or equal operator
   bool operator>=(const TimeSpan& rhs) const { return !operator<(rhs); }  ///< less or equal operator

   /// sets time span components
   void SetDateTimeSpan(int iHours, int iMins, int iSecs, int iMillisecs);

   /// format time span as ISO 8601 (in "hh:mm:ss" format)
   CString FormatISO8601() const;

private:
   /// prepares another copy of the object when date/time value is about to be modified
   void PrepareCopy();

private:
   friend class DateTime;

   /// implementation
   std::shared_ptr<class TimeSpanImpl> m_spImpl;
};
