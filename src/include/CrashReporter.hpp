//
// ulib - a collection of useful classes
// Copyright (C) 2000-2017 Michael Fink
//
/// \file CrashReporter.hpp Application crash reporting
//
#pragma once

/// crash reporter helper class
class CrashReporter
{
public:
   /// function type that can show a crash dialog
   typedef void (*T_fnShowCrashDialog)(LPCTSTR);

   /// sets up crash reporting
   static void Init(const CString& appName, const CString& basePath,
      T_fnShowCrashDialog fnShowCrashDialog = T_fnShowCrashDialog());
};
