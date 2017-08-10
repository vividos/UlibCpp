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

#include <atlbase.h>

#if _ATL_VER < 0x0700
#error ATL7 or higher must be used to compile
#endif

#include <atlcoll.h>
#include <atlstr.h>
#include <atltypes.h>
#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES

#include <atlwin.h>
#include <atlcom.h>
#include <atlhost.h>

// atlimage.h declares throw() on methods that may throw; this would lead to calling
// std::unexpected(), which would quit the application; define throw() as empty macro in order to
// get the different behavior that exceptions are propagated to the caller.
#define throw()
#pragma warning(disable: 4002) // too many actual parameters for macro 'throw'
#include <atlimage.h>
#undef throw
#pragma warning(pop)

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
