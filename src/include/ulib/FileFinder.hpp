//
// ulib - a collection of useful classes
// Copyright (C) 2008-2017 Michael Fink
//
/// \file FileFinder.hpp File finder
//
#pragma once

#include <vector>

/// file finder
class FileFinder
{
public:
   /// finds all files in given path, with a file spec (e.g. *.png) and returns them
   /// \param[in] path path to use for searching
   /// \param[in] fileSpec file specification, e.g. *.png or *.*
   /// \param[in] findFolders indicates if folders should be found instead of files
   /// \param[in] recursive indicates if all subfolders should be searched recursively, too
   /// \return list of found full filenames (or folder names)
   static std::vector<CString> FindAllInPath(const CString& path, const CString& fileSpec, bool findFolders, bool recursive);

   /// ctor; starts finding files
   FileFinder(const CString& baseFolder, const CString& fileSpec)
      :m_findHandle(INVALID_HANDLE_VALUE),
       m_baseFolder(baseFolder)
   {
      if (m_baseFolder.Right(1) != _T("\\"))
         m_baseFolder += _T("\\");

      ::ZeroMemory(&m_findData, sizeof(m_findData));
      m_findHandle = ::FindFirstFile(m_baseFolder + fileSpec, &m_findData);
   }

   /// dtor
   ~FileFinder()
   {
      if (m_findHandle != INVALID_HANDLE_VALUE)
      {
         ATLVERIFY(TRUE == ::FindClose(m_findHandle));
      }
   }

   /// indicates if any files were found
   bool IsValid() const { return m_findHandle != INVALID_HANDLE_VALUE; }

   /// returns if the current file entry is a dot file, "." or ".."
   bool IsDot() const
   {
      ATLASSERT(IsValid() == true);

      if ((m_findData.cFileName[0] == _T('.') && m_findData.cFileName[1] == _T('\0')) ||
          (m_findData.cFileName[0] == _T('.') && m_findData.cFileName[1] == _T('.') && m_findData.cFileName[2] == _T('\0')))
         return true;

      return false;
   }

   /// returns if current file entry is a file
   bool IsFile() const
   {
      return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
   }

   /// returns if current file entry is a folder
   bool IsFolder() const
   {
      return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
   }

   /// returns complete filename of current file entry
   CString Filename() const
   {
      CString filename = m_baseFolder + m_findData.cFileName;

      if (IsFolder())
         filename += _T("\\");

      return filename;
   }

   /// retrieves next file entry
   bool Next()
   {
      ATLASSERT(IsValid() == true);
      return TRUE == ::FindNextFile(m_findHandle, &m_findData);
   }

private:
   /// base folder of search
   CString m_baseFolder;

   /// current find entry
   WIN32_FIND_DATA m_findData;

   /// find handle
   HANDLE m_findHandle;
};
