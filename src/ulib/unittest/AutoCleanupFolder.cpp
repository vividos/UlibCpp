//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2017 Michael Fink
//
/// \file AutoCleanupFolder.cpp auto-deleting folder
//

// includes
#include "stdafx.h"
#include "unittest/AutoCleanupFolder.hpp"
#include "Path.hpp"
#include "FileFinder.hpp"
#include <deque>

using UnitTest::AutoCleanupFolder;

/// Creates a temporary folder in the "unittest" subfolder of the
/// TEMP folder of the current user. Under Windows CE the \\Temp folder is
/// used.
AutoCleanupFolder::AutoCleanupFolder()
{
   // search folder name
   CString basePath;
#ifdef _WIN32_WCE
   basePath = _T("\\Temp\\unittest");
#else
   basePath = Path::Combine(Path::TempFolder(), _T("unittest")).ToString();
#endif

   Path::CreateDirectoryRecursive(basePath);
   Path::AddEndingBackslash(basePath);

   unsigned int counter = 0;

   do
   {
      m_folderName.Format(_T("%stest-%06x"), basePath.GetString(), counter++);

   } while(INVALID_FILE_ATTRIBUTES != ::GetFileAttributes(m_folderName));

   Path::CreateDirectoryRecursive(m_folderName);
   Path::AddEndingBackslash(m_folderName);
}

/// Cleans up folder contents, and all subfolders. Also tries to remove parent
/// folder; this is done to eventually clean up the created base folder; this
/// might fail, though, since there could be more unit test folders.
AutoCleanupFolder::~AutoCleanupFolder()
{
   std::deque<CString> dequeFolders;
   dequeFolders.push_back(m_folderName);

   while (!dequeFolders.empty())
   {
      CString folder = dequeFolders[0];
      FileFinder finder(folder, _T("*.*"));

      if (finder.IsValid())
      {
         do
         {
            if (finder.IsDot())
               continue;

            if (finder.IsFolder())
            {
               // fonder, add to folder to process
               dequeFolders.push_back(finder.Filename());
            }
            else
            {
               // no, plain file; delete it
               ::DeleteFile(finder.Filename());
            }


         } while (finder.Next());
      }

      folder.TrimRight(_T('\\'));
      ::RemoveDirectory(folder);

      dequeFolders.pop_front();
   }

   // try to remove parent dir, in case we're the last folder
   m_folderName.TrimRight(_T('\\'));
   CString parentFolder = Path(m_folderName).FolderName();

   if (!parentFolder.IsEmpty())
      ::RemoveDirectory(parentFolder);
}
