//
// ulib - a collection of useful classes
// Copyright (C) 2004,2005,2006,2007,2008,2018 Michael Fink
//
/// \file VersionInfoResource.cpp version info resource class
//
#include "stdafx.h"
#include <ulib/win32/VersionInfoResource.hpp>
#include <winbase.h> // for VOS_* constants
#include <winver.h> // for VOS_* constants, when not in winbase.h

#ifndef VOS_WINDOWSCE
#  define VOS_WINDOWSCE 0x00050000L
#endif

#ifndef _WIN32_WCE
#  pragma comment(lib, "version.lib")
#endif

using Win32::FixedFileInfo;
using Win32::VersionInfoResource;
using Win32::LANGANDCODEPAGE;

//
// FixedFileInfo
//

CString FixedFileInfo::GetFileVersion() const
{
   CString text;
   text.Format(_T("%u.%u.%u.%u"),
      HIWORD(dwFileVersionMS), LOWORD(dwFileVersionMS),
      HIWORD(dwFileVersionLS), LOWORD(dwFileVersionLS));
   return text;
}

CString FixedFileInfo::GetProductVersion() const
{
   CString text;
   text.Format(_T("%u.%u.%u.%u"),
      HIWORD(dwProductVersionMS), LOWORD(dwProductVersionMS),
      HIWORD(dwProductVersionLS), LOWORD(dwProductVersionLS));
   return text;
}

CString FixedFileInfo::GetFileOS() const
{
   CString text;

   switch (dwFileOS & 0xFFFF0000)
   {
   case VOS_DOS: // The file was designed for MS-DOS.
      text = _T("VOS_DOS"); break;
   case VOS_OS216: // The file was designed for 16-bit OS/2.
      text = _T("VOS_OS216"); break;
   case VOS_OS232: // The file was designed for 32-bit OS/2.
      text = _T("VOS_OS232"); break;
   case VOS_NT: // The file was designed for Windows NT and Windows 2000.
      text = _T("VOS_NT"); break;
   case VOS_WINDOWSCE:
      text = _T("VOS_WINDOWSCE"); break;
   case VOS_UNKNOWN:
      text = _T("VOS_UNKNOWN"); break;
   default:
      text = _T("unknown"); break;
   }

   switch (dwFileOS & 0x0000FFFF)
   {
   case VOS__WINDOWS16: // The file was designed for 16-bit Windows.
      text += _T(" | VOS__WINDOWS16"); break;
   case VOS__PM16: // The file was designed for 16-bit Presentation Manager.
      text += _T(" | VOS__PM16"); break;
   case VOS__PM32: // The file was designed for 32-bit Presentation Manager.
      text += _T(" | VOS__PM32"); break;
   case VOS__WINDOWS32: // The file was designed for the Win32 API.
      text += _T(" | VOS__WINDOWS32"); break;
   }

   return text;
}

CString FixedFileInfo::GetFileType() const
{
   CString text;
   switch (dwFileType)
   {
   case VFT_UNKNOWN:
      text = _T("VFT_UNKNOWN"); break;
   case VFT_APP:
      text = _T("VFT_APP"); break;
   case VFT_DLL:
      text = _T("VFT_DLL"); break;
   case VFT_DRV:
      text = _T("VFT_DRV"); break;
   case VFT_FONT:
      text = _T("VFT_FONT"); break;
   case VFT_VXD:
      text = _T("VFT_VXD"); break;
   case VFT_STATIC_LIB:
      text = _T("VFT_STATIC_LIB"); break;
   default:
      text = _T("unknown"); break;
   }
   return text;
}

//
// VersionInfoResource
//

VersionInfoResource::VersionInfoResource(LPCTSTR filename)
{
   LPTSTR filename2 = const_cast<LPTSTR>(filename);

   // find out length
   DWORD dummy = 0;
   DWORD dwLen = ::GetFileVersionInfoSize(filename2, &dummy);

   if (dwLen > 0)
   {
      m_versionInfoData.resize(dwLen);

      // get version info
      ::GetFileVersionInfo(filename2, 0, dwLen, &m_versionInfoData[0]);
   }
}

LPVOID VersionInfoResource::QueryValue(LPCTSTR subBlock, UINT* puiSize)
{
   ATLASSERT(IsAvail());

   LPTSTR subBlock2 = const_cast<LPTSTR>(subBlock);

   LPVOID data = NULL;
   UINT uiSize = 0;

   BOOL ret = VerQueryValue(&m_versionInfoData[0],
      subBlock2,
      &data,
      puiSize != NULL ? puiSize : &uiSize);

   return ret == FALSE ? NULL : data;
}

void VersionInfoResource::GetLangAndCodepages(std::vector<LANGANDCODEPAGE>& langAndCodepageList)
{
   ATLASSERT(IsAvail());

   UINT size = 0;
   LPVOID data = QueryValue(_T("\\VarFileInfo\\Translation"), &size);

   ATLASSERT((size % sizeof(LANGANDCODEPAGE)) == 0);

   LANGANDCODEPAGE* langAndCodepage = reinterpret_cast<LANGANDCODEPAGE*>(data);

   for (unsigned int i = 0; i < size / sizeof(LANGANDCODEPAGE); i++)
      langAndCodepageList.push_back(langAndCodepage[i]);
}

CString VersionInfoResource::GetStringValue(const LANGANDCODEPAGE& langAndCodepage, LPCTSTR valueName)
{
   CString buffer;
   buffer.Format(
      _T("\\StringFileInfo\\%04x%04x\\%s"),
      langAndCodepage.wLanguage, langAndCodepage.wCodePage,
      valueName);

   UINT size = 0;
   LPCWSTR text = reinterpret_cast<LPCWSTR>(QueryValue(buffer, &size));

   ATLASSERT(size == 0 || text[size - 1] == 0);

   return (text == NULL || *text == 0) ? CString(_T("???")) : CString(text);
}
