//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2017,2020 Michael Fink
//
/// \file AutoCleanupFile.hpp auto-deleting file for unit tests
//
#pragma once

/// \brief unit test classes
namespace UnitTest
{
   /// \brief auto-deleting file
   /// \details This class automatically deletes the given filename when it goes out of scope.
   class AutoCleanupFile
   {
   public:
      /// ctor
      explicit AutoCleanupFile(LPCTSTR filename)
         :m_filename(filename)
      {
      }

      /// copy ctor; not available
      AutoCleanupFile(const AutoCleanupFile&) = delete;

      /// move ctor; not available
      AutoCleanupFile(AutoCleanupFile&&) = delete;

      /// dtor
      ~AutoCleanupFile()
      {
         ::DeleteFile(m_filename);
      }

      /// copy assignment operator; not available
      AutoCleanupFile& operator=(const AutoCleanupFile&) = delete;

      /// move assignment operator; not available
      AutoCleanupFile& operator=(AutoCleanupFile&&) = delete;

   private:
      /// filename
      LPCTSTR m_filename;
   };

} // namespace UnitTest
