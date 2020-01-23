//
// ulib - a collection of useful classes
// Copyright (C) 2004,2005,2006,2007,2008,2017,2020 Michael Fink
//
/// \file Path.cpp Path class
//

#include "stdafx.h"
#include <ulib/Path.hpp>

#pragma warning(disable: 4996) // deprecated method warnings/errors

const TCHAR Path::Separator[2] = _T("\\");

bool Path::Canonicalize(CString& path)
{
   CString newPath;
   BOOL ret = ::PathCanonicalize(newPath.GetBuffer(MAX_PATH), path);
   newPath.ReleaseBuffer();

   if (ret != FALSE)
      path = newPath;

   return ret != FALSE;
}

/// \note: deprecated, to be removed
bool Path::Canonicalize()
{
   return Path::Canonicalize(m_path);
}

CString Path::Combine(const CString& part1, const CString& part2)
{
   CString newPath = part1;
   AddEndingBackslash(newPath);
   return newPath + part2;
}

/// \note: deprecated, to be removed
Path Path::Combine(const CString& part2)
{
   CString part1 = m_path;

   AddEndingBackslash(part1);

   return Path(part1 + part2);
}

/// \note: deprecated, to be removed
CString Path::FilenameAndExt() const
{
   return Path::FilenameAndExt(m_path);
}

CString Path::FilenameAndExt(const CString& path)
{
   int pos = path.ReverseFind(Path::SeparatorCh);
   if (pos == -1)
      return path;

   return path.Mid(pos + 1);
}

/// \note: deprecated, to be removed
CString Path::FilenameOnly() const
{
   return Path::FilenameOnly(m_path);
}

CString Path::FilenameOnly(const CString& path)
{
   int pos = path.ReverseFind(Path::SeparatorCh);

   int pos2 = path.ReverseFind(_T('.'));
   if (pos2 == -1)
      return path.Mid(pos + 1);

   return path.Mid(pos + 1, pos2 - pos - 1);
}

/// \note: deprecated, to be removed
CString Path::ExtensionOnly() const
{
   return Path::ExtensionOnly(m_path);
}

CString Path::ExtensionOnly(const CString& path)
{
   int pos = path.ReverseFind(Path::SeparatorCh);

   int pos2 = path.ReverseFind(_T('.'));
   if (pos2 == -1 || pos2 < pos)
      return CString();

   return path.Mid(pos2);
}

/// \note: deprecated, to be removed
CString Path::FolderName() const
{
   return Path::FolderName(m_path);
}

CString Path::FolderName(const CString& path)
{
   int pos = path.ReverseFind(Path::SeparatorCh);
   if (pos == -1)
      return path;

   return path.Left(pos + 1);
}

/// \note: deprecated, to be removed
CString Path::ShortPathName() const
{
   return Path::ShortPathName(m_path);
}

CString Path::ShortPathName(const CString& path)
{
   CString shortPathName;
   DWORD ret = ::GetShortPathName(path, shortPathName.GetBuffer(MAX_PATH), MAX_PATH);
   shortPathName.ReleaseBuffer();

   return ret == 0 ? path : shortPathName;
}

/// \note: deprecated, to be removed
CString Path::MakeRelativeTo(const CString& rootPath)
{
   return Path::MakeRelativeTo(m_path, rootPath);
}

CString Path::MakeRelativeTo(const CString& path, const CString& rootPath)
{
   CString relativePath;

   DWORD pathAttr = ::GetFileAttributes(path) & FILE_ATTRIBUTE_DIRECTORY;

   BOOL ret = ::PathRelativePathTo(
      relativePath.GetBuffer(MAX_PATH),
      rootPath, FILE_ATTRIBUTE_DIRECTORY,
      path, pathAttr);

   relativePath.ReleaseBuffer();

   return ret == FALSE ? CString() : relativePath;
}

/// \note: deprecated, to be removed
bool Path::IsRelative() const
{
   return Path::IsRelative(m_path);
}

bool Path::IsRelative(const CString& path)
{
   BOOL ret = ::PathIsRelative(path);
   return ret != FALSE;
}

/// \note: deprecated, to be removed
bool Path::FileExists() const
{
   return Path::FileExists(m_path);
}

bool Path::FileExists(const CString& path)
{
   DWORD ret = ::GetFileAttributes(path);
   if (ret == INVALID_FILE_ATTRIBUTES)
      return false; // doesn't exist

   if ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0)
      return false; // no, it's a folder

   return true;
}

/// \note: deprecated, to be removed
bool Path::FolderExists() const
{
   return Path::FolderExists(m_path);
}

bool Path::FolderExists(const CString& path)
{
   DWORD ret = ::GetFileAttributes(path);
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

CString Path::GetCommonRootPath(const CString& path1, const CString& path2)
{
   Path canonPath1(path1);
   Path canonPath2(path2);
   canonPath1.Canonicalize();
   canonPath2.Canonicalize();

   if (::PathIsSameRoot(canonPath1.ToString(), canonPath2.ToString()) != TRUE)
   {
      return CString();
   }

   CString commonRootPath;
   PathCommonPrefix(canonPath1.ToString(), canonPath2.ToString(), commonRootPath.GetBuffer(MAX_PATH));
   commonRootPath.ReleaseBuffer();

   Path::AddEndingBackslash(commonRootPath);

   return commonRootPath;
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

CString Path::ModuleFilename(HMODULE moduleHandle)
{
   CString filename;

   ::GetModuleFileName(moduleHandle, filename.GetBuffer(MAX_PATH), MAX_PATH);
   filename.ReleaseBuffer();

   return filename;
}

bool Path::CreateDirectoryRecursive(LPCTSTR directoryName)
{
   CString parentDirectory = directoryName;

   if (parentDirectory.IsEmpty())
      return false;

   parentDirectory.TrimRight(Path::SeparatorCh);
   parentDirectory = Path::FolderName(parentDirectory);

   if (parentDirectory == directoryName)
      return false;

   if (!Path::FolderExists(parentDirectory))
      CreateDirectoryRecursive(parentDirectory);

   return ::CreateDirectory(directoryName, nullptr) != FALSE;
}
