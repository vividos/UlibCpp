//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2017 Michael Fink
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
      /// dtor; cleans up folder
      ~AutoCleanupFolder();

      /// returns folder name; always ends with a slash or backslash
      const CString& FolderName() const
      {
         return m_folderName;
      }

   private:
      /// folder name
      CString m_folderName;
   };

} // namespace UnitTest
