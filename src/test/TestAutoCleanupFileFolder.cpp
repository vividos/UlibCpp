//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012,2017 Michael Fink
//
/// \file TestAutoCleanupFileFolder.cpp tests for AutoCleanupFile and AutoCleanupFolder classes
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/unittest/AutoCleanupFolder.hpp>
#include <ulib/unittest/AutoCleanupFile.hpp>
#include <cstdio>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests for AutoCleanupFile and AutoCleanupFolder
   TEST_CLASS(TestAutoCleanupFileFolder)
   {
      /// tests AutoCleanupFolder class
      TEST_METHOD(TestAutoCleanupFolder)
      {
         CString folderName;
         {
            AutoCleanupFolder folder;
            folderName = folder.FolderName();

            Assert::IsTrue(INVALID_FILE_ATTRIBUTES != ::GetFileAttributes(folderName));
         }

         Assert::IsTrue(INVALID_FILE_ATTRIBUTES == ::GetFileAttributes(folderName));
      }

      /// tests AutoCleanupFile class
      TEST_METHOD(TestAutoCleanupFile)
      {
         AutoCleanupFolder folder;

         CString filename(folder.FolderName() + _T("test.txt"));
         {
            FILE* fd = NULL;
            _tfopen_s(&fd, filename, _T("wb"));
            Assert::IsNotNull(fd, _T("file must be open"));
            char c = 42;
            fwrite(&c, 1, 1, fd);
            fclose(fd);

            AutoCleanupFile file(filename);

            Assert::IsTrue(INVALID_FILE_ATTRIBUTES != ::GetFileAttributes(filename));
         }

         Assert::IsTrue(INVALID_FILE_ATTRIBUTES == ::GetFileAttributes(filename));
      }
   };

} // namespace UnitTest
