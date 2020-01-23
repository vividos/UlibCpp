//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2013,2017,2019,2020 Michael Fink
//
/// \file Thread.hpp Thread class
//

#pragma once

/// thread helper class
class Thread
{
public:
   /// sets thread name for current or specified thread; unicode version
   static void SetName(LPCWSTR threadName, DWORD threadId = DWORD(-1))
   {
      Thread::SetName(CStringA(threadName), threadId);
   }

   /// sets thread name for current or specified thread; ansi version
   /// \note from http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
   static void SetName(LPCSTR threadName, DWORD threadId = DWORD(-1));

   /// returns current thread ID
   static DWORD CurrentId();
};
