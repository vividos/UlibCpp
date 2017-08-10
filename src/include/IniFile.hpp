//
// ulib - a collection of useful classes
// Copyright (C) 2000-2017 Michael Fink
//
/// \file IniFile.hpp Ini file wrapper
//
#pragma once

/// ini file wrapper
class IniFile
{
public:
   /// ctor
   IniFile(const CString& iniFilename)
      :m_iniFilename(iniFilename)
   {
   }

   /// returns integer value from section and key
   int GetInt(LPCTSTR sectionName, LPCTSTR keyName, int defaultValue)
   {
      return ::GetPrivateProfileInt(sectionName, keyName, defaultValue, m_iniFilename);
   }

   /// returns string value from section and key
   CString GetString(LPCTSTR sectionName, LPCTSTR keyName, LPCTSTR defaultValue)
   {
      CString value;
      ::GetPrivateProfileString(sectionName, keyName, defaultValue, value.GetBuffer(512), 512, m_iniFilename);
      value.ReleaseBuffer();

      return value;
   }

   /// writes a string value to a section and key
   void WriteString(LPCTSTR sectionName, LPCTSTR keyName, LPCTSTR value)
   {
      ::WritePrivateProfileString(sectionName, keyName, value, m_iniFilename);
   }

private:
   /// ini filename
   CString m_iniFilename;
};
