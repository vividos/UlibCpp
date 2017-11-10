//
// ulib - a collection of useful classes
// Copyright (C) 2014-2017 Michael Fink
//
/// \file Path.hpp Path class
//

#pragma once

/// file and folder path class
class Path
{
public:
   // ctors

   /// default ctor
   explicit Path()
   {
      // no further initialisation necessary
   }

   /// ctor that takes a path
   explicit Path(const CString& path)
      :m_path(path)
   {
   }

   // operators

   /// returns CString
   operator const CString&() const { return m_path; }
   /// returns raw string pointer
   operator LPCTSTR() const { return m_path; }

   // getters

   /// converts path to string
   CString ToString() const { return m_path; }

   /// returns filename and extension
   CString FilenameAndExt() const;

   /// returns filename without extension
   CString FilenameOnly() const;

   /// returns extension only, with leading dot
   CString ExtensionOnly() const;

   /// returns folder name, without filename, but ending slash
   CString FolderName() const;

   /// returns short path name (filename in 8.3 format)
   CString ShortPathName() const;

   /// make this path relative to the given root path and returns it
   CString MakeRelativeTo(const CString& rootPath);

   /// returns if stored path is a relative path
   bool IsRelative() const;

   /// returns if path represents a file and if it exists
   bool FileExists() const;

   /// returns if path represents a folder and if it exists
   bool FolderExists() const;

   // methods

   /// canonicalizes path by removing '..', etc.
   bool Canonicalize();

   /// combine path with given second part and return new path
   Path Combine(const CString& part2);

   /// adds a backslash at the end of the path
   static void AddEndingBackslash(CString& path);

   /// combine both path parts and return new path
   static Path Combine(const CString& part1, const CString& part2)
   {
      Path path1(part1);
      return path1.Combine(part2);
   }

   /// returns the common root path of both given paths; returns empty string when there's no common root
   static CString GetCommonRootPath(const CString& path1, const CString& path2);

   /// returns special folder; see CSIDL_* constants
   static CString SpecialFolder(int csidl);

   /// returns the windows folder
   static CString WindowsFolder();

   /// returns the temp folder
   static CString TempFolder();

   /// returns file name of given module name (e.g. kernel32.dll); nullptr means the currently running .exe module
   static CString ModuleFilename(HMODULE moduleHandle);

   /// creates a directory, possibly also creating non-existent parent directories
   static bool CreateDirectoryRecursive(LPCTSTR directoryName);

   // public members

   /// path separator string
   static const TCHAR Separator[2];

   /// path separator character
   static const TCHAR SeparatorCh = _T('\\');

private:
   /// path
   CString m_path;
};
