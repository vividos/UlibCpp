//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014 Michael Fink
//
/// \file ulib/config/Common.hpp common configuration
//
#pragma once

#include <ulib/config/CompileTimeWarningError.hpp>

#ifndef UNUSED
/// Use this to mark any (potentially) unused function parameter names in the
/// function body.
#define UNUSED(x) (void)(x)
#endif

/// determines the size of an array, in elements; uses C++11
template <typename T>
inline constexpr auto sizeof_array(const T& theArray)
{
   return (sizeof(theArray) / sizeof(theArray[0]));
}
