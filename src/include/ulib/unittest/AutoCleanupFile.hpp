//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2017 Michael Fink
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
      AutoCleanupFile(LPCTSTR filename)
         :m_filename(filename)
      {
      }
      /// dtor
      ~AutoCleanupFile()
      {
         ::DeleteFile(m_filename);
      }

   private:
      /// filename
      LPCTSTR m_filename;
   };

} // namespace UnitTest
