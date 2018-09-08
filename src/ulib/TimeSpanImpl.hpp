//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file TimeSpanImpl.hpp time span implementation class
//

#include "stdafx.h"
#include <ulib/TimeSpan.hpp>
#include <ulib/config/BoostDateTime.hpp>

/// \brief time span implementation
/// \details uses boost::date_time classes
class TimeSpanImpl
{
public:
   /// ctor; initializes using special value
   explicit TimeSpanImpl(boost::date_time::special_values sv = boost::date_time::not_a_date_time)
      :m_span(sv)
   {
   }

   /// ctor; initializes using given time_duration object
   explicit TimeSpanImpl(const boost::posix_time::time_duration& span)
      :m_span(span)
   {
   }

   /// time duration object
   boost::posix_time::time_duration m_span;
};
