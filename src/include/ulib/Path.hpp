//
// ulib - a collection of useful classes
// Copyright (C) 2014-2017,2020 Michael Fink
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
   [[deprecated("Please use the static Path methods instead of Path objects")]]
   explicit Path()
   {
      // no further initialisation necessary
   }

   /// ctor that takes a path
   [[deprecated("Please use the static Path methods instead of Path objects")]]
   explicit Path(const CString& path)
      :m_path(path)
   {
   }

   // operators

   /// returns CString
   [[deprecated("Please use the static Path methods instead of Path objects")]]
   operator const CString& () const { return m_path; }
   /// returns raw string pointer
   [[deprecated("Please use the static Path methods instead of Path objects")]]
   operator LPCTSTR() const { return m_path; }

   // getters

   /// converts path to string
   [[deprecated("Please use the static Path methods instead of Path objects")]]
   CString ToString() const { return m_path; }

   /// returns filename and extension
   [[deprecated("Please use the static Path::FilenameAndExt() method instead")]]
   CString FilenameAndExt() const;

   /// returns filename without extension
   [[deprecated("Please use the static Path::FilenameOnly() method instead")]]
   CString FilenameOnly() const;

   /// returns extension only, with leading dot
   [[deprecated("Please use the static Path::ExtensionOnly() method instead")]]
   CString ExtensionOnly() const;

   /// returns folder name, without filename, but ending slash
   [[deprecated("Please use the static Path::FolderName() method instead")]]
   CString FolderName() const;

   /// returns short path name (filename in 8.3 format)
   [[deprecated("Please use the static Path::ShortPathName() method instead")]]
   CString ShortPathName() const;

   /// make this path relative to the given root path and returns it
   [[deprecated("Please use the static Path::MakeRelativeTo() method instead")]]
   CString MakeRelativeTo(const CString& rootPath);

   /// returns if stored path is a relative path
   [[deprecated("Please use the static Path::IsRelative() method instead")]]
   bool IsRelative() const;

   /// returns if path represents a file and if it exists
   [[deprecated("Please use the static Path::FileExists() method instead")]]
   bool FileExists() const;

   /// returns if path represents a folder and if it exists
   [[deprecated("Please use the static Path::FolderExists() method instead")]]
   bool FolderExists() const;

   // methods

   /// canonicalizes path by removing '..', etc.
   [[deprecated("Please use the static Path::Canonicalize() method instead")]]
   bool Canonicalize();

   /// combine path with given second part and return new path
   [[deprecated("Please use the static Path::Combine() method instead")]]
   Path Combine(const CString& part2);

   /// returns filename and extension
   static CString FilenameAndExt(const CString& path);

   /// returns filename without extension
   static CString FilenameOnly(const CString& path);

   /// returns extension only, with leading dot
   static CString ExtensionOnly(const CString& path);

   /// returns folder name, without filename, but ending slash
   static CString FolderName(const CString& path);

   /// returns short path name (filename in 8.3 format); file must actually exist
   static CString ShortPathName(const CString& path);

   /// make path relative to the given root path and returns it
   static CString MakeRelativeTo(const CString& path, const CString& rootPath);

   /// returns if stored path is a relative path
   static bool IsRelative(const CString& path);

   /// returns if path represents a file and if it exists
   static bool FileExists(const CString& path);

   /// returns if path represents a folder and if it exists
   static bool FolderExists(const CString& path);

   /// canonicalizes path by removing '..', etc.
   static bool Canonicalize(CString& path);

   /// adds a backslash at the end of the path
   static void AddEndingBackslash(CString& path);

   /// combine both path parts and return new path
   static CString Combine(const CString& part1, const CString& part2);

   /// returns the common root path of both given paths; returns empty string when there's no common root
   static CString GetCommonRootPath(const CString& path1, const CString& path2);

   /// returns special folder; see CSIDL_* constants
   static CString SpecialFolder(int csidl);

   /// returns the windows folder
   static CString WindowsFolder();

   /// returns the temp folder
   static CString TempFolder();

   /// returns file name of given module name (e.g. kernel32.dll); nullptr means the currently running .exe module
   static CString ModuleFilename(HMODULE moduleHandle = nullptr);

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
