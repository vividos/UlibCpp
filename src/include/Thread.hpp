//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2013,2017 Michael Fink
//
/// \file Thread.hpp Thread class
//

#pragma once

/// thread helper class
class Thread
{
public:
   /// sets thread name for current or specified thread; unicode version
   static void SetName(LPCWSTR pszThreadName, DWORD dwThreadId = DWORD(-1))
   {
      Thread::SetName(CStringA(pszThreadName), dwThreadId);
   }

   /// sets thread name for current or specified thread; ansi version
   /// \note from http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
   static void SetName(LPCSTR pszThreadName, DWORD dwThreadId = DWORD(-1));

   /// returns current thread ID
   static DWORD CurrentId()
   {
      return ::GetCurrentThreadId();
   }
};
