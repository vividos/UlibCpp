//
// ulib - a collection of useful classes
// Copyright (C) 2000-2017 Michael Fink
//
/// \file CrashReporter.cpp Application crash reporting
//
#include "stdafx.h"
#include <ulib/CrashReporter.hpp>
#include <ulib/Path.hpp>
#include <strsafe.h>
#include <ctime>
#include <memory>

#pragma warning(disable: 4091) // 'typedef ': ignored on left of '' when no variable is declared
#include <dbghelp.h>
#pragma warning(default: 4091)

#pragma comment(lib, "dbghelp.lib")

/// base path for writing minidump file
static TCHAR g_minidumpBasePath[MAX_PATH] = { 0 };

/// function to call to show a crash dialog; may be null
static CrashReporter::T_fnShowCrashDialog g_fnShowCrashDialog = nullptr;

/// writes minidump file
bool WriteMinidump(HANDLE fileHandle, _EXCEPTION_POINTERS* exceptionInfo)
{
   __try
   {
      MINIDUMP_EXCEPTION_INFORMATION miniDumpExceptionInfo = { 0 };
      miniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
      miniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
      miniDumpExceptionInfo.ClientPointers = false;

      BOOL ret = MiniDumpWriteDump(
         GetCurrentProcess(),
         GetCurrentProcessId(),
         fileHandle,
         MiniDumpNormal,
         &miniDumpExceptionInfo,
         nullptr,
         nullptr);

      return ret != FALSE;
   }
   __except (EXCEPTION_EXECUTE_HANDLER)
   {
      return false;
   }
}

/// creates minidump filename in given buffer
void GetMinidumpFilename(LPTSTR minidumpFilename, UINT numMaxChars)
{
   ATLVERIFY(S_OK == StringCchCopy(
      minidumpFilename,
      numMaxChars,
      g_minidumpBasePath));

   size_t lenBasePath = _tcslen(minidumpFilename);
   TCHAR* start = minidumpFilename + lenBasePath;
   size_t numRemaining = numMaxChars - lenBasePath;

   // add date
   time_t nowt = ::time(&nowt);

   struct tm now = { 0 };
   localtime_s(&now, &nowt);

   _sntprintf_s(start, numRemaining, numRemaining,
      _T("%04u-%02u-%02u %02u_%02u_%02u.mdmp"),
      now.tm_year + 1900,
      now.tm_mon + 1,
      now.tm_mday,
      now.tm_hour,
      now.tm_min,
      now.tm_sec);
}

/// exception filter function to write minidump file
LONG WINAPI ExceptionFilterWriteMinidump(_EXCEPTION_POINTERS* exceptionInfo)
{
   OutputDebugString(_T("!!! ExceptionFilterWriteMinidump() called!\n"));

   // construct filename
   TCHAR minidumpFilename[MAX_PATH];
   GetMinidumpFilename(minidumpFilename, sizeof(minidumpFilename) / sizeof(*minidumpFilename));

   // write minidump file
   HANDLE fileHandle = CreateFile(
      minidumpFilename,
      GENERIC_WRITE,
      0,
      nullptr,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      nullptr);

   if (INVALID_HANDLE_VALUE == fileHandle)
      return EXCEPTION_CONTINUE_SEARCH;

   std::shared_ptr<void> file(fileHandle, &::CloseHandle);

   OutputDebugString(_T("!!! Writing minidump to file: "));
   OutputDebugString(minidumpFilename);
   OutputDebugString(_T("\n"));

   WriteMinidump(fileHandle, exceptionInfo);

   file.reset();

   ATLTRACE(_T("wrote minidump file: %s\n"), minidumpFilename);

   if (g_fnShowCrashDialog != nullptr)
      g_fnShowCrashDialog(minidumpFilename);

   return EXCEPTION_EXECUTE_HANDLER;
}

/// handler function for std::terminate
void OnStdTerminate()
{
   OutputDebugString(_T("!!! OnStdTerminate() called!\n"));

   // cause an exception, so that we can write a minidump
   EXCEPTION_POINTERS* exceptionInfo = nullptr;
   __try
   {
      throw 42;
   }
   __except ((exceptionInfo = GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
   {
      if (exceptionInfo != nullptr)
         ExceptionFilterWriteMinidump(exceptionInfo);
   }
}

void CrashReporter::Init(const CString& appName, const CString& basePath, T_fnShowCrashDialog fnShowCrashDialog)
{
   g_fnShowCrashDialog = fnShowCrashDialog;

   // set minidump base path
   CString path = basePath;
   Path::AddEndingBackslash(path);

   ::CreateDirectory(path, nullptr);

   path += appName + _T("-");

   ATLVERIFY(S_OK == StringCchCopy(
      g_minidumpBasePath,
      sizeof(g_minidumpBasePath) / sizeof(*g_minidumpBasePath),
      path));

   // set exception filter
   SetUnhandledExceptionFilter(&ExceptionFilterWriteMinidump);

   // catch all std::terminate() calls
   std::set_terminate(&OnStdTerminate);
}
