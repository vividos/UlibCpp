//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2017,2020 Michael Fink
//
/// \file AutoCleanupFolder.hpp auto-deleting folder
//
#pragma once

namespace UnitTest
{
   /// \brief auto-deleting folder for unit tests
   /// \details This class automatically deletes the content of a temporary folder
   class AutoCleanupFolder
   {
   public:
      /// ctor; creates folder
      AutoCleanupFolder();

      /// copy ctor; not available
      AutoCleanupFolder(const AutoCleanupFolder&) = delete;

      /// move ctor; not available
      AutoCleanupFolder(AutoCleanupFolder&&) = delete;

      /// dtor; cleans up folder
      ~AutoCleanupFolder();

      /// copy assignment operator; not available
      AutoCleanupFolder& operator=(const AutoCleanupFolder&) = delete;

      /// move assignment operator; not available
      AutoCleanupFolder& operator=(AutoCleanupFolder&&) = delete;

      /// returns folder name; always ends with a slash or backslash
      const CString& FolderName() const
      {
         return m_folderName;
      }

   private:
      /// cleans up folder
      void CleanUp();

   private:
      /// folder name
      CString m_folderName;
   };

} // namespace UnitTest
