//
// ulib - a collection of useful classes
// Copyright (C) 2008-2017 Michael Fink
//
/// \file Win7Taskbar.cpp Windows 7 task bar access
//
#include "stdafx.h"
#include <ulib/win32/Win7Taskbar.hpp>
#include <ShObjIdl.h>

using Win32::TaskbarProgressBar;
using Win32::TaskbarImpl;
using Win32::Taskbar;

/// task bar implementation
class TaskbarImpl
{
public:
   /// ctor
   TaskbarImpl(HWND hwnd)
      :m_hwnd(hwnd)
   {
   }

   HWND m_hwnd;   ///< window handle of window
   CComPtr<ITaskbarList3> m_taskBarList;  ///< task bar list interface 3
};

void TaskbarProgressBar::SetState(TaskbarProgressBarState state)
{
   ATLASSERT(
      state == TBPF_NOPROGRESS ||
      state == TBPF_INDETERMINATE ||
      state == TBPF_NORMAL ||
      state == TBPF_ERROR ||
      state == TBPF_PAUSED);

   m_impl->m_taskBarList->SetProgressState(
      m_impl->m_hwnd,
      static_cast<TBPFLAG>(state));
}

void TaskbarProgressBar::SetPos(UINT currentPos, UINT maxPos)
{
   m_impl->m_taskBarList->SetProgressValue(m_impl->m_hwnd, currentPos, maxPos);
}

Taskbar::Taskbar(HWND hwnd)
   :m_impl(new TaskbarImpl(hwnd))
{
   HRESULT hr = m_impl->m_taskBarList.CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_ALL);
   ATLVERIFY(SUCCEEDED(hr));
}

bool Taskbar::IsAvailable() const
{
   return m_impl->m_taskBarList != nullptr;
}
