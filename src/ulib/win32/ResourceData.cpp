//
// ulib - a collection of useful classes
// Copyright (C) 2004,2005,2006,2007,2008,2017 Michael Fink
//
/// \file ResourceData.cpp resource data class
//

// includes
#include "stdafx.h"
#include <ulib/win32/ResourceData.hpp>

using Win32::ResourceData;

ResourceData::ResourceData(LPCTSTR resourceName, LPCTSTR resourceType, HINSTANCE instanceHandle)
   :m_resourceName(resourceName),
   m_resourceType(resourceType),
   m_global(nullptr),
   m_size(0),
   m_instanceHandle(instanceHandle)
{
}

bool ResourceData::IsAvailable() const
{
   HRSRC resource = FindResource(m_instanceHandle, m_resourceName, m_resourceType);
   return resource != nullptr;
}

bool ResourceData::AsRawData(std::vector<BYTE>& rawData)
{
   DWORD size = 0;
   LPVOID data = GetResource(size);
   if (data == nullptr)
      return false;

   rawData.resize(size);

   memcpy(&rawData[0], data, size);

   CloseResource();
   return true;
}


CString ResourceData::AsString(bool storedAsUnicode)
{
   DWORD size = 0;
   LPVOID data = GetResource(size);
   if (data == nullptr)
      return CString();

   // convert to string
   CString text;

   if (storedAsUnicode)
      text = CString(static_cast<LPCWSTR>(data)), size / sizeof(WCHAR);
   else
      text = CString(static_cast<LPCSTR>(data)), size / sizeof(CHAR);

   CloseResource();

   return text;
}

bool ResourceData::AsFile(LPCTSTR filename)
{
   DWORD size = 0;
   LPVOID data = GetResource(size);
   if (data == nullptr)
      return false;

   // save as file
   {
      HANDLE fileHandle = CreateFile(filename, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
      if (fileHandle == nullptr)
         return false;

      DWORD dwBytesWritten = 0;
      WriteFile(fileHandle, data, size, &dwBytesWritten, nullptr);
      ATLASSERT(size == dwBytesWritten);

      CloseHandle(fileHandle);
   }

   CloseResource();
   return true;
}

LPVOID ResourceData::GetResource(DWORD& size)
{
   HRSRC resource = FindResource(m_instanceHandle, m_resourceName, m_resourceType);
   if (resource == nullptr)
      return nullptr;

   size = SizeofResource(m_instanceHandle, resource);
   if (size == 0)
      return nullptr;

   m_global = LoadResource(m_instanceHandle, resource);
   if (m_global == nullptr)
      return nullptr;

   LPVOID data = LockResource(m_global);
   return data;
}

void ResourceData::CloseResource()
{
   if (m_global != nullptr)
   {
      // normally not needed on win32, but for completeness;
      // on WinCE these two doesn't exists
#ifndef _WIN32_WCE
      UnlockResource(m_global);
      FreeResource(m_global);
#endif
   }
}
