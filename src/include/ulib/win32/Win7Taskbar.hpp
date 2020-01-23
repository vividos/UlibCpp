//
// ulib - a collection of useful classes
// Copyright (C) 2008-2020 Michael Fink
//
/// \file Win7Taskbar.hpp Windows 7 Taskbar classes
//
#pragma once

#include <memory>

namespace Win32
{
   class TaskbarImpl;

   /// Taskbar progress bar access
   class TaskbarProgressBar
   {
   public:
      /// state of task bar progress bar
      enum TaskbarProgressBarState
      {
         TBPF_NOPROGRESS = 0,
         TBPF_INDETERMINATE = 0x1,
         TBPF_NORMAL = 0x2,
         TBPF_ERROR = 0x4,
         TBPF_PAUSED = 0x8
      };

      /// copy ctor
      TaskbarProgressBar(const TaskbarProgressBar& other)
         :m_impl(other.m_impl)
      {
      }

      // move ctor
      TaskbarProgressBar(TaskbarProgressBar&& other) noexcept
         :m_impl(std::move(other.m_impl))
      {
      }

      /// dtor; returns progress to "none"
      ~TaskbarProgressBar()
      {
         SetState(TBPF_NOPROGRESS);
      }

      /// copy assignment operator
      TaskbarProgressBar& operator=(const TaskbarProgressBar& other)
      {
         if (this != &other)
            m_impl = other.m_impl;
         return *this;
      }

      /// move assignment operator; not available
      TaskbarProgressBar& operator=(TaskbarProgressBar&& other) noexcept
      {
         m_impl = std::move(other.m_impl);
         return *this;
      }

      /// sets new progress bar state
      void SetState(TaskbarProgressBarState state);

      /// sets new progress bar position
      void SetPos(UINT currentPos, UINT maxPos);

   private:
      friend class Taskbar;

      /// ctor; can only be called from Taskbar
      explicit TaskbarProgressBar(const std::shared_ptr<TaskbarImpl>& impl)
         :m_impl(impl)
      {
         SetState(TBPF_INDETERMINATE);
      }

   private:
      /// implementation
      std::shared_ptr<TaskbarImpl> m_impl;
   };

   /// Windows 7 taskbar access
   class Taskbar
   {
   public:
      /// accesses task bar; uses task bar icon associated with given window
      explicit Taskbar(HWND hwnd);

      /// returns if task bar is available (Windows 7 and higher)
      bool IsAvailable() const;

      /// dtor
      ~Taskbar()
      {
         // nothing to cleanup
      }

      TaskbarProgressBar OpenProgressBar()
      {
         ATLASSERT(IsAvailable());
         return TaskbarProgressBar(m_impl);
      }

   private:
      std::shared_ptr<TaskbarImpl> m_impl;
   };

} // namespace Win32
