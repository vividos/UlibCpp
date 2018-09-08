//
// ulib - a collection of useful classes
// Copyright (C) 2012,2017 Michael Fink
//
/// \file ISO8601Parser.cpp ISO 8601 date parser
//
#include "stdafx.h"
#include "ISO8601Parser.hpp"

ISO8601Parser::ISO8601Parser(LPCTSTR timestamp)
   :m_dt(boost::date_time::not_a_date_time)
{
   std::wstring strRemaining;
   if (!ParseDateTime(timestamp, strRemaining))
      return;

   if (!ParseTimezoneOffset(strRemaining))
      return;

   if (m_tzOffset.is_not_a_date_time())
   {
      m_dt = boost::date_time::not_a_date_time;
      return;
   }
}

bool ISO8601Parser::ParseDateTime(LPCTSTR timestamp, std::wstring& strRemaining)
{
   // convert to ptime using time input facet
   // http://stackoverflow.com/questions/2838524/use-boost-date-time-to-parse-and-create-http-dates
   typedef boost::local_time::wlocal_time_input_facet time_input_facet;

   // note: inputFacet is not deleted here; it is done somewhere inside the stream classes
   // note: we don't use
   //    inputFacet->set_iso_extended_format();
   // here, since it doesn't include the T divider
   time_input_facet* inputFacet =
      new time_input_facet(L"%Y-%m-%dT%H:%M:%S%F");

   std::wstringstream ss;
   ss.imbue(std::locale(ss.getloc(), inputFacet));

   ss.str(timestamp);

   boost::local_time::local_date_time localDateTime =
      boost::local_time::local_sec_clock::local_time(boost::local_time::time_zone_ptr());

   ss >> localDateTime;

   if (ss.fail())
      return false;

   // convert to ptime
   m_dt = localDateTime.utc_time();

   std::getline(ss, strRemaining);

   return true;
}

bool ISO8601Parser::ParseTimezoneOffset(std::wstring& strRemaining)
{
   if (strRemaining.empty())
      return false;

   // check next char
   TCHAR chNext = strRemaining[0];
   if (chNext == _T('Z'))
   {
      // UTC
      m_tzOffset = boost::posix_time::time_duration(0, 0, 0);
   }
   else if (chNext == _T('+') || chNext == _T('-'))
   {
      // numeric offset
      std::string strRemaining2(CStringA(strRemaining.c_str()).GetString());
      m_tzOffset = boost::posix_time::duration_from_string(strRemaining2);
   }

   return !m_tzOffset.is_not_a_date_time();
}
