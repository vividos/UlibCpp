//
// ulib - a collection of useful classes
// Copyright (C) 2014,2017 Michael Fink
//
/// \file Process.hpp Win32 process
//
#pragma once

/// \brief Win32 specific classes
namespace Win32
{
   /// Win32 process
   class Process
   {
   public:
      /// ctor
      Process()
      {
         ZeroMemory(&m_startupInfo, sizeof(m_startupInfo));
         m_startupInfo.cb = sizeof(m_startupInfo);

         ZeroMemory(&m_processInfo, sizeof(m_processInfo));
      }

      /// dtor
      ~Process()
      {
         if (m_processInfo.hThread != nullptr)
            CloseHandle(m_processInfo.hThread);

         if (m_processInfo.hProcess != nullptr)
            CloseHandle(m_processInfo.hProcess);
      }

      /// sets working directory for process
      void WorkingDirectory(const CString& workingDirectory)
      {
         m_workingDirectory = workingDirectory;
         m_workingDirectory.TrimRight(_T('\\'));
      }

      /// creates process with given command line
      bool Create(const CString& commandLine)
      {
#pragma warning (disable: 6335) // Leaking process information handle 'm_processInfo.hProcess'.
         BOOL ret = ::CreateProcess(
            nullptr,
            const_cast<LPTSTR>(static_cast<LPCTSTR>(commandLine)),
            nullptr, nullptr, FALSE, 0, nullptr,
            m_workingDirectory.IsEmpty() ? nullptr : m_workingDirectory.GetString(),
            &m_startupInfo,
            &m_processInfo);
#pragma warning (default: 6335)

         return ret != FALSE;
      }

      /// returns process handle
      HANDLE ProcessHandle() const { return m_processInfo.hProcess; }

   private:
      /// startup info
      STARTUPINFO m_startupInfo;

      /// process info
      PROCESS_INFORMATION m_processInfo;

      /// working directory to start in
      CString m_workingDirectory;
   };

} // namespace Win32
