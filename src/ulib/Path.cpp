//
// ulib - a collection of useful classes
// Copyright (C) 2004,2005,2006,2007,2008,2017 Michael Fink
//
/// \file Path.cpp Path class
//

#include "stdafx.h"
#include <ulib/Path.hpp>

const TCHAR Path::Separator[2] = _T("\\");

bool Path::Canonicalize()
{
   CString newPath;
   BOOL ret = ::PathCanonicalize(newPath.GetBuffer(MAX_PATH), m_path);
   newPath.ReleaseBuffer();

   if (ret != FALSE)
      m_path = newPath;

   return ret != FALSE;
}

Path Path::Combine(const CString& part2)
{
   CString part1 = m_path;

   AddEndingBackslash(part1);

   return Path(part1 + part2);
}

CString Path::FilenameAndExt() const
{
   int pos = m_path.ReverseFind(Path::SeparatorCh);
   if (pos == -1)
      return m_path;

   return m_path.Mid(pos + 1);
}

CString Path::FilenameOnly() const
{
   int pos = m_path.ReverseFind(Path::SeparatorCh);

   int pos2 = m_path.ReverseFind(_T('.'));
   if (pos2 == -1)
      return m_path.Mid(pos + 1);

   return m_path.Mid(pos + 1, pos2 - pos - 1);
}

CString Path::FolderName() const
{
   int pos = m_path.ReverseFind(Path::SeparatorCh);
   if (pos == -1)
      return m_path;

   return m_path.Left(pos + 1);
}

CString Path::ShortPathName() const
{
   CString shortPathName;
   DWORD ret = ::GetShortPathName(m_path, shortPathName.GetBuffer(MAX_PATH), MAX_PATH);
   shortPathName.ReleaseBuffer();

   return ret == 0 ? m_path : shortPathName;
}

CString Path::MakeRelativeTo(const CString& rootPath)
{
   CString relativePath;

   DWORD pathAttr = ::GetFileAttributes(m_path) & FILE_ATTRIBUTE_DIRECTORY;

   BOOL ret = ::PathRelativePathTo(
      relativePath.GetBuffer(MAX_PATH),
      rootPath, FILE_ATTRIBUTE_DIRECTORY,
      m_path, pathAttr);

   relativePath.ReleaseBuffer();

   return ret == FALSE ? CString() : relativePath;
}

bool Path::IsRelative() const
{
   BOOL ret = ::PathIsRelative(m_path);
   return ret != FALSE;
}

bool Path::FileExists() const
{
   DWORD ret = ::GetFileAttributes(m_path);
   if (ret == INVALID_FILE_ATTRIBUTES)
      return false; // doesn't exist

   if ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0)
      return false; // no, it's a folder

   return true;
}

bool Path::FolderExists() const
{
   DWORD ret = ::GetFileAttributes(m_path);
   if (ret == INVALID_FILE_ATTRIBUTES)
      return false; // doesn't exist

   if ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0)
      return true; // yes, it's a folder

   return false;
}

void Path::AddEndingBackslash(CString& path)
{
   if (path.Right(1) != Path::Separator)
      path += Path::Separator;
}

CString Path::SpecialFolder(int csidl)
{
   CString specialFolder;

   ::SHGetFolderPath(nullptr, csidl, nullptr, SHGFP_TYPE_CURRENT, specialFolder.GetBuffer(MAX_PATH));
   specialFolder.ReleaseBuffer();

   return specialFolder;
}

CString Path::WindowsFolder()
{
   CString windowsFolder;

   UINT ret = ::GetWindowsDirectory(windowsFolder.GetBuffer(MAX_PATH), MAX_PATH);
   windowsFolder.ReleaseBuffer(ret);

   return windowsFolder;
}

CString Path::TempFolder()
{
   CString tempFolder;

   DWORD ret = ::GetTempPath(MAX_PATH, tempFolder.GetBuffer(MAX_PATH));
   tempFolder.ReleaseBuffer(ret);

   return tempFolder;
}

bool Path::CreateDirectoryRecursive(LPCTSTR directoryName)
{
   CString parentDirectory = directoryName;

   if (parentDirectory.IsEmpty())
      return false;

   parentDirectory.TrimRight(Path::SeparatorCh);
   parentDirectory = Path(parentDirectory).FolderName();

   if (parentDirectory == directoryName)
      return false;

   if (!Path(parentDirectory).FolderExists())
      CreateDirectoryRecursive(parentDirectory);

   return ::CreateDirectory(directoryName, nullptr) != FALSE;
}
