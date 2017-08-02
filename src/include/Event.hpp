//
// ulib - a collection of useful classes
// Copyright (C) 2008,2013,2017 Michael Fink
//
/// \file Event.hpp event class
//
#pragma once

#include "SystemException.hpp"
#include <memory>

/// event class
template <bool manualReset>
class EventT
{
public:
   /// ctor
   EventT(bool initialState)
   {
      HANDLE eventHandle = ::CreateEvent(
         nullptr, manualReset ? TRUE : FALSE, initialState ? TRUE : FALSE, nullptr);

      if (eventHandle == NULL)
         throw SystemException(_T("failed to create event"), GetLastError(), __FILE__, __LINE__);

      m_event.reset(eventHandle, ::CloseHandle);
   }

   /// sets event
   void Set()
   {
      BOOL ret = ::SetEvent(m_event.get());
      if (ret == FALSE)
         throw SystemException(_T("failed to set event"), GetLastError(), __FILE__, __LINE__);
   }

   /// resets event
   void Reset()
   {
      BOOL ret = ::ResetEvent(m_event.get());
      if (ret == FALSE)
         throw SystemException(_T("failed to set event"), GetLastError(), __FILE__, __LINE__);
   }

   /// waits given time (or infinitely) for event to get set
   bool Wait(DWORD timeoutInMilliseconds = INFINITE)
   {
      DWORD ret = ::WaitForSingleObject(m_event.get(), timeoutInMilliseconds);
      if (ret == WAIT_OBJECT_0)
         return true;

      if (ret == WAIT_TIMEOUT)
         return false;

      throw SystemException(_T("failed to wait for event"), ret, __FILE__, __LINE__);
   }

   /// returns internal event handle
   HANDLE Handle() const { return m_event != NULL ? m_event.get() : INVALID_HANDLE_VALUE; }

private:
   /// event handle
   std::shared_ptr<void> m_event;
};

/// event that has to be reset manually when set
typedef EventT<true> ManualResetEvent;

/// event that auto-resets as soon as a thread successfully Wait()ed on it
typedef EventT<true> AutoResetEvent;
