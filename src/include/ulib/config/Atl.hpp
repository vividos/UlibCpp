//
// ulib - a collection of useful classes
// Copyright (C) 2000-2017 Michael Fink
//
/// \file Atl.hpp ATL includes
//
#pragma once

// ignore /analyze warnings in ATL header files
#pragma warning(push)
#pragma warning(disable: 6001 6031 6387 26110)

/// prevent ATL using stuff removed by WIN32_LEAN_AND_MEAN
#define ATL_NO_LEAN_AND_MEAN

/// no min-max macros, we use std::min / std::max instead
#define NOMINMAX 1

// min and max macros are used in atlcom.h, etc., so temporarily define them here
#ifndef min
#  define min(x,y) (x) < (y) ? (x) : (y) ///< temporary define of min()
#endif

#ifndef max
#  define max(x,y) (x) > (y) ? (x) : (y) ///< temporary define of min()
#endif

// ATL includes
#include <atlbase.h>

#if _ATL_VER < 0x0700
#error ATL7 or higher must be used to compile
#endif

#include <atlcoll.h>
#include <atlstr.h>
#include <atltypes.h>
#include <atlwin.h>
#include <atlcom.h>
#include <atlhost.h>

// for _stdcallthunk
#include <atlstdthunk.h>

// atlimage.h declares throw() on methods that may throw; this would lead to calling
// std::unexpected(), which would quit the application; define throw() as empty macro in order to
// get the different behavior that exceptions are propagated to the caller.
#define throw()
#pragma warning(disable: 4002) // too many actual parameters for macro 'throw'
#include <atlimage.h>
#undef throw
#pragma warning(pop)

// undef the macros so that std::min and std::max work as they should be
#undef min
#undef max

// redefine ATLVERIFY when analyzing using Coverity Scan
#if !defined(_DEBUG) && defined(__COVERITY__)
#undef ATLVERIFY
#define ATLVERIFY(expr) (void)(expr)
#endif
