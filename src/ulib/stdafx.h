//
// ulib - a collection of useful classes
// Copyright (C) 2017,2020 Michael Fink
//
/// \file ulib/stdafx.h Precompiled header support
//
#pragma once

// including SDKDDKVer.h defines the highest available Windows platform.
#ifdef _WIN32
#include <sdkddkver.h>
#endif

#include <ulib/config/Common.hpp>

#ifdef _WIN32
#include <ulib/config/Win32.hpp>
#include <ulib/config/Atl.hpp>
#else
#include <ulib/config/Android.hpp>
#endif

// don't link against Boost.DateTime; only header functionality is used
#define BOOST_DATE_TIME_NO_LIB

// redefine ATLVERIFY when analyzing using Coverity Scan
#if !defined(_DEBUG) && defined(__COVERITY__)
#undef ATLVERIFY
#define ATLVERIFY(expr) (void)(expr)
#endif
