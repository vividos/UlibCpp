//
// ulib - a collection of useful classes
// Copyright (C) 2012,2017 Michael Fink
//
/// \file ISO8601Parser.hpp ISO 8601 date parser
//
#pragma once

#pragma warning(disable: 6054) // String 'x' might not be zero-terminated.
#pragma warning(disable: 6330) // warning C6330: 'char' passed as _Param_(1) when 'unsigned char' is required in call to 'isspace'.
#pragma warning(disable: 28251) // Inconsistent annotation for 'F': this instance has no annotations
#include <boost/date_time.hpp>
#pragma warning(default: 6054)
#pragma warning(default: 6330)
#pragma warning(default: 28251)

/// \brief parser for ISO 8601 date formats
class ISO8601Parser
{
public:
   /// ctor; starts parsing
   explicit ISO8601Parser(LPCTSTR pszTimestamp);

   /// returns if parsed date/time is valid
   bool IsValid() const
   {
      return !m_dt.is_not_a_date_time() && !m_tzOffset.is_not_a_date_time();
   }

   /// returns parsed date/time
   boost::posix_time::ptime Get() const
   {
      return m_dt - m_tzOffset;
   }

private:
   /// parses date/time part
   bool ParseDateTime(LPCTSTR pszTimestamp, std::wstring& strRemaining);

   /// parses timezone offset
   bool ParseTimezoneOffset(std::wstring& strRemaining);

private:
   /// date/time part
   boost::posix_time::ptime m_dt;

   /// timezone offset
   boost::posix_time::time_duration m_tzOffset;
};
