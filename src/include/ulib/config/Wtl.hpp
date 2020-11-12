//
// ulib - a collection of useful classes
// Copyright (C) 2009,2012,2017,2018,2019,2020 Michael Fink
//
/// \file Wtl.hpp configuration for WTL 8 or higher
//
#pragma once

// needed includes
#include <ulib/config/Atl.hpp>

// ignore /analyze warnings in WTL header files
#pragma warning(push)
#pragma warning(disable: 6001 6011 6031 6294 6385 6387 26110 26444 26451 26454 26495 26812 26819 28182)

// ignore warnings in Win32 API header files that WTL includes
#pragma warning(disable: 4091) // 'typedef ' : ignored on left of 'T1' when no variable is declared

// ignore constexpr warnings in WTL headers
#pragma warning(disable: 4127) // conditional expression is constant

#define _WTL_NO_CSTRING ///< don't use WTL CString
#define _WTL_NO_WTYPES ///< don't use WTL types, such as CSize, CRect, etc.

// WTL includes
#include <atlapp.h>
extern CAppModule _Module; ///< app module

#include <atlmisc.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <shellapi.h> // needed for ShellExecute, used in atlctrlx.h
#include <atlctrlx.h>
#include <atlddx.h>
#include <atlcrack.h>
#include <atlsplit.h>

#if _WIN32_WINNT >= _WIN32_WINNT_WIN7
   #if _WTL_VER < 0x1000
      #error ulib only supports WTL 10 and higher; please upgrade!
   #else
      #include <atlribbon.h>
   #endif
#endif

#pragma warning(pop)

// add manifest for common controls 6
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
