//
// ulib - a collection of useful classes
// Copyright (C) 2017 Michael Fink
//
/// \file ulib/stdafx.h Precompiled header support
//
#pragma once

// including SDKDDKVer.h defines the highest available Windows platform.
#include <SDKDDKVer.h>

#include <ulib/config/Common.hpp>
#include <ulib/config/Win32.hpp>
#include <ulib/config/Atl.hpp>

// don't link against Boost.DateTime; only header functionality is used
#define BOOST_DATE_TIME_NO_LIB
